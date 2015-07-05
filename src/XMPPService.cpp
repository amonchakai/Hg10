/*
 * XMPPController.cpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */


#include "XMPPService.hpp"
#include "QXmppVCardIq.h"

#include <QDir>
#include <QFile>
#include <QBuffer>
#include <QDebug>
#include <QRegExp>
#include <QReadWriteLock>
#include <bb/cascades/ImageTracker>
#include <bb/system/SystemDialog>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>
#include <QTimer>

#include "DataObjects.h"
#include "ConversationManager.hpp"


QReadWriteLock  mutex;
QReadWriteLock  mutexLoadLocal;

XMPP* XMPP::m_This = NULL;

XMPP::XMPP(QObject *parent) : QObject(parent),
        m_Datas(new QList<Contact*>()),
        m_PushStack(NULL),
        m_Connected(false),
        m_PictureRecovery(false),
        m_ClientSocket(new QTcpSocket(this)), m_ScheduleContactListRequest(false),
        m_NbFails(0) {

    bool check = connect(m_ClientSocket, SIGNAL(connected()), this, SLOT(connected()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = connect(m_ClientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionToServiceFailed(QAbstractSocket::SocketError)));
    Q_ASSERT(check);

    loadLocal();
    connectToXMPPService();

}

// get singleton
XMPP *XMPP::get() {
    if(m_This == NULL) {
        // if not already done, instantiate the network manager
        m_This = new XMPP(NULL);
    }

    return m_This;
}

const QList<Contact*>* XMPP::getContacts() {
    if(m_Datas->empty()) {
        if(m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState)
            getContactList();
        else
            m_ScheduleContactListRequest = true;
    }


    return m_Datas;
}



void XMPP::connected() {

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Connected to XMPP headless!" << m_ClientSocket->state() ;

        if(m_NbFails != 0) {
            askConnectionStatus();
            //getContactList();
        }

        if(m_ScheduleContactListRequest)
            getContactList();

        requestPresence();
    }
}

void XMPP::connectionToServiceFailed(QAbstractSocket::SocketError e) {
    qDebug() << "Connection error to headless service: " << e << " restart in 1s...";
    if(e == QAbstractSocket::RemoteHostClosedError && m_NbFails < 2) {
        QTimer::singleShot(1000, this, SLOT(connectToXMPPService()));
        ++m_NbFails;
        return;
    }



    bool result = tryRestartHeadless();
    m_NbFails = 0;

    if(result == false) {
        bb::system::SystemDialog *dialog = new bb::system::SystemDialog("OK");
        dialog->setTitle(tr("Something bad :("));
        dialog->setBody(tr("The connection to the headless service cannot be established.\n\nMaybe you did not allow it? \nMaybe it crashed...\n\nYou can check the permission, try to kill/restart the process, reinstall the app, reboot your device... \n\nDelete and forget this stupid app\'"));
        dialog->show();
    } else {
        QTimer::singleShot(3000, this, SLOT(connectToXMPPService()));
    }
}

void XMPP::disconnectToXMPPService() {
    qDebug() << "UI hidden";
    //return;
    if (m_ClientSocket->isOpen() || (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState)) {
        qDebug() << "disconnect UI from headless service";
        m_ClientSocket->disconnectFromHost();
    }
}

void XMPP::connectToXMPPService() {
    if (!m_ClientSocket->isOpen() || (m_ClientSocket && m_ClientSocket->state() != QTcpSocket::ConnectedState)) {
        qDebug() << "Request connection to Headless service";
        m_ClientSocket->connectToHost(QHostAddress::LocalHost, 27015);
        bool ok = connect(m_ClientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        Q_ASSERT(ok);
        ok = connect(m_ClientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
    } else {
        qDebug() << "Already connected" << m_ClientSocket->state();
        connected();
    }
}

void XMPP::disconnected() {

}

void XMPP::askConnectionStatus() {
    mutex.lockForWrite();
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "To headless: XMPPServiceMessages::REQUEST_CONNECTION_STATUS";
        int code = XMPPServiceMessages::REQUEST_CONNECTION_STATUS;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->flush();
    }
    mutex.unlock();
}

void XMPP::readyRead() {

    QByteArray code_str = m_ClientSocket->read(sizeof(int));
    while(code_str.size() == sizeof(int)) {
        int code = *reinterpret_cast<int*>(code_str.data());
        if(code != 9)
            qDebug() << "COMMAND: " << code;

        switch(code) {

            case XMPPServiceMessages::REPLY_CONTACT_LIST: {
                qDebug() << "list received";
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString name(m_ClientSocket->read(length));

                code_str = m_ClientSocket->read(sizeof(int));
                int status = *reinterpret_cast<int*>(code_str.data());

                loadvCard(name, true, status);

                emit offline(false);

            }
                break;


            case XMPPServiceMessages::REPLY_LOGGED_IN: {
                emit connectedXMPP();
                qDebug() << "XMPPServiceMessages::REPLY_LOGGED_IN";
            }
                break;

            case XMPPServiceMessages::REPLY_CONNECTION_FAILED: {
                emit connectionFailed();
                qDebug() << "XMPPServiceMessages::REPLY_CONNECTION_FAILED";

            }
                break;


            case XMPPServiceMessages::STATUS_UPDATE: {
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString from(m_ClientSocket->read(length));

                code_str = m_ClientSocket->read(sizeof(int));
                int code = *reinterpret_cast<int*>(code_str.data());

                ConversationManager::get()->updateState(from, code);

            }
                break;



            case XMPPServiceMessages::PRESENCE_UPDATE: {
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString from(m_ClientSocket->read(length));

                code_str = m_ClientSocket->read(sizeof(int));
                int code = *reinterpret_cast<int*>(code_str.data());

                mutex.lockForWrite();
                for(int i = 0 ; i < m_Datas->size() ; ++i) {
                    if(m_Datas->at(i)->getID().toLower() == from.toLower()) {
                        m_Datas->at(i)->setPresence(code);
                        break;
                    }
                }
                mutex.unlock();

                emit presenceUpdated(from, code);

            }
                break;


            case XMPPServiceMessages::MESSAGE: {
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString from(m_ClientSocket->read(length));

                code_str = m_ClientSocket->read(sizeof(int));
                length = *reinterpret_cast<int*>(code_str.data());
                QString to(m_ClientSocket->read(length));

                code_str = m_ClientSocket->read(sizeof(int));
                length = *reinterpret_cast<int*>(code_str.data());
                QString body(QTextCodec::codecForName("UTF-8")->toUnicode(m_ClientSocket->read(length)));

                ConversationManager::get()->receiveMessage(from, to, body);

            }
                break;


            case XMPPServiceMessages::OTR_GONE_SECURE: {
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString with(m_ClientSocket->read(length));

                emit goneSecure(with);
            }
                break;

            case XMPPServiceMessages::OTR_GONE_UNSECURE: {
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString with(m_ClientSocket->read(length));

                emit goneUnsecure(with);
            }
                break;

            case XMPPServiceMessages::OTR_FINGERPRINT_RECEIVED: {
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString from(m_ClientSocket->read(length));

                code_str = m_ClientSocket->read(sizeof(int));
                length = *reinterpret_cast<int*>(code_str.data());
                QString fingerprint(m_ClientSocket->read(length));

                emit fingerprintReceived(from, fingerprint);
            }
                break;

            case XMPPServiceMessages::OTR_OWN_FINGERPRINT: {
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString fingerprint(m_ClientSocket->read(length));

                emit ownFingerprint(fingerprint);
            }
                break;

            case XMPPServiceMessages::OTR_WAS_NOT_SECURE: {
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString from(m_ClientSocket->read(length));

                code_str = m_ClientSocket->read(sizeof(int));
                length = *reinterpret_cast<int*>(code_str.data());
                QString body(QTextCodec::codecForName("UTF-8")->toUnicode(m_ClientSocket->read(length)));

                emit wasNotSecure(from, body);
            }
                break;
        }

        code_str = m_ClientSocket->read(sizeof(int));
    }

}

// ===========================================================================================


void XMPP::connectToServer(const QString &user, const QString& password) {
    mutex.lockForWrite();
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::LOGIN;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        code = user.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->write(user.toAscii());

        code = password.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->write(password.toAscii());
        m_ClientSocket->flush();
    }
    mutex.unlock();

}

void XMPP::advancedConnectToServer( const QString& host,
                                    const QString &domain,
                                    int port,
                                    const QString &login,
                                    const QString &password,
                                    int encryption,
                                    int proxy) {
    mutex.lockForWrite();
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::ADVANCED_LOGIN;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        code = host.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->write(host.toAscii());

        code = domain.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->write(domain.toAscii());

        m_ClientSocket->write(reinterpret_cast<char*>(&port), sizeof(int));

        code = login.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->write(login.toAscii());

        code = password.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->write(password.toAscii());

        m_ClientSocket->write(reinterpret_cast<char*>(&encryption), sizeof(int));
        m_ClientSocket->write(reinterpret_cast<char*>(&proxy),      sizeof(int));

        m_ClientSocket->flush();
    }
    mutex.unlock();
}

void XMPP::oauth2Login(const QString &user) {
    qDebug() << "Request login oauth2";
    mutex.lockForWrite();
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::OAUTH2_LOGIN;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        code = user.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->write(user.toAscii());

        m_ClientSocket->flush();
    }
    mutex.unlock();
}


void XMPP::disconnectFromServer() {
    mutex.lockForWrite();
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::DISCONNECT;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->flush();
    }
    mutex.unlock();
}


// ===========================================================================================


void XMPP::getContactList() {
    mutex.lockForWrite();
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::REQUEST_CONTACT_LIST;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->flush();
        m_Datas->clear();
    }
    mutex.unlock();
}


void XMPP::loadLocal() {
    qDebug() << "Load local data";
    mutexLoadLocal.lockForWrite();

    bool delayPush = false;

    QString directory = QDir::homePath() + QLatin1String("/vCards");
    if (QFile::exists(directory)) {
        QDir dir(directory);
        dir.setNameFilters(QStringList() << "*.xml");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList()) {

            //dir.remove(dirFile);
            //qDebug() << dirFile;
            loadvCard(dirFile.mid(0,dirFile.length()-4));
        }
    }

    mutexLoadLocal.unlock();

    if(!delayPush)
        emit contactReceived();

}

void XMPP::requestPresence() {
    mutex.lockForWrite();
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "XMPPServiceMessages::REQUEST_CONTACT_LIST_PRESENCE";

        int code = XMPPServiceMessages::REQUEST_CONTACT_LIST_PRESENCE;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->flush();
        // m_Datas->clear(); ?
    }
    mutex.unlock();


}

void XMPP::clear() {
    m_Datas->clear();
    ConversationManager::get()->clear();
}



void XMPP::loadvCard(const QString &bareJid, bool push, int status) {
    m_PictureRecovery = false;

    //if(bareJid.isEmpty())
    //    return;


    // -------------------------------------------------------------
    // get vCard from file
    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");
    QFile file(vCardsDir + "/" + bareJid + ".xml");

    QDomDocument doc("vCard");
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        //file.close();
        //return;
        qWarning() << "Problem while reading vcard: " << bareJid << " file not complete, some data may be missing.";

    }
    file.close();


    QXmppVCardIq vCard;
    vCard.parse(doc.documentElement());

    // --------------------------------------------------------------
    // insert vCard content to view

    Contact *contact = new Contact;
    contact->setID(vCard.from());

    QString photoName = bareJid;



    if(!QFile::exists(vCardsDir + "/" + photoName + ".png")) {

        // ----------------------------------------------------------------------------------
        //  get the photo from the vCard, and write an image.


        QString name(vCardsDir + "/" + bareJid + ".png");

        QByteArray photo = vCard.photo();
        QImage qImage;
        qImage.loadFromData(vCard.photo());


        if(!qImage.isNull() && qImage.size().height() > qImage.size().width()) {
            QImage nqImage = qImage.scaled(qImage.size().height(), qImage.size().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            qImage = nqImage;
        }

        if(!qImage.isNull() && qImage.size().height() < 64 && qImage.size().width()) {
            //QPixmap pixMap = QPixmap::fromImage(qImage.convertToFormat(QImage::Format_ARGB4444_Premultiplied)); /*
            QImage nqImage = qImage.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            qImage = nqImage;
        }


        uchar *bits = qImage.bits();
        int radius = std::min(qImage.size().width(), qImage.size().height())/2; radius = radius*radius;
        int center_x = qImage.size().width() / 2;
        int center_y = qImage.size().height() / 2;
        int depth = qImage.depth() / 8;

        // save two representation of the picture: a square for the post, and a disk for the user list
        if(!photo.isEmpty()) {
            qImage.save(name + ".square.png", "PNG");
        }


        for(int i = 0 ; i < qImage.size().width() ; ++i) {
            for(int j = 0 ; j < qImage.size().height() ; ++j) {
                int dstCenter = (center_x - i)*(center_x - i) + (center_y - j)*(center_y - j);
                if(dstCenter > radius) {
                    for(int c = 0 ; c < depth ; ++c) {
                        bits[(j*qImage.size().width()+i)*depth+c] = 255*(c != 3);
                    }
                }
            }
        }

        qImage.convertToFormat(QImage::Format_ARGB4444_Premultiplied).save(name, "PNG");
    }


    if(QFile::exists(vCardsDir + "/" + photoName + ".png"))
        contact->setAvatar(vCardsDir + "/" + photoName + ".png");
    else
        contact->setAvatar("asset:///images/avatar.png");




    if(!vCard.fullName().isEmpty())
        contact->setName(vCard.fullName());
    else {
        if(!vCard.nickName().isEmpty())
            contact->setName(vCard.nickName());
        else {
            if(!vCard.firstName().isEmpty())
                contact->setName(vCard.firstName() + " ");
            if(!vCard.lastName().isEmpty())
                contact->setName(contact->getName() + vCard.lastName());

            if(contact->getName().isEmpty())
                contact->setName(bareJid);
        }
    }
    contact->setTimestamp(0);
    contact->setPresence(status);
    m_Datas->push_back(contact);


    mutex.lockForWrite();
    //if(push && !(vCard.fullName().isEmpty() && vCard.nickName().isEmpty()) && !delayPush) {
        emit pushContact(contact);
    //}
    mutex.unlock();

}

void XMPP::sendMessageTo(const QString &to, const QString &message) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::SEND_MESSAGE;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        int length = to.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(to.toAscii(), length);

        QByteArray sentMess = message.toUtf8();
        length = sentMess.size();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(sentMess.data(), length);

        m_ClientSocket->flush();
    }

    mutex.unlock();
}

void XMPP::setupKeys() {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::OTR_SETUP_KEYS;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        m_ClientSocket->flush();
    }

    mutex.unlock();
}

void XMPP::showKeys() {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::OTR_GET_OWN_FINGERPRINT;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        m_ClientSocket->flush();
    }

    mutex.unlock();
}

void XMPP::requestOTRStatus(const QString& to) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::OTR_REQUEST_STATUS;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        int length = to.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(to.toAscii(), length);

        m_ClientSocket->flush();
    }

    mutex.unlock();
}

void XMPP::requestOTRSession(const QString& to) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::OTR_REQUEST_START;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        int length = to.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(to.toAscii(), length);

        m_ClientSocket->flush();
    }

    mutex.unlock();
}

void XMPP::closeOTRSession(const QString& to) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::OTR_REQUEST_STOP;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        int length = to.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(to.toAscii(), length);

        m_ClientSocket->flush();
    }

    mutex.unlock();
}

void XMPP::notifySettingChange() {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::REFRESH_SETTINGS;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        m_ClientSocket->flush();
    }

    mutex.unlock();
}


void XMPP::setStatusText(const QString &text, int presence) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::SET_STATUS_TEXT;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        int length = text.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(text.toAscii(), length);

        m_ClientSocket->write(reinterpret_cast<char*>(&presence), sizeof(int));

        m_ClientSocket->flush();
    }

    mutex.unlock();
}


void XMPP::setPresence(int presence) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::SET_PRESENCE;
        m_ClientSocket->write(reinterpret_cast<char*>(&code),     sizeof(int));
        m_ClientSocket->write(reinterpret_cast<char*>(&presence), sizeof(int));

        m_ClientSocket->flush();
    }

    mutex.unlock();
}


void XMPP::addContact(const QString &email) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::ADD_CONTACT;
        m_ClientSocket->write(reinterpret_cast<char*>(&code),   sizeof(int));

        int length = email.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(email.toAscii(), length);

        m_ClientSocket->flush();
    }

    mutex.unlock();
}


// -------------------------------------------------------------
// file transfer handling

void XMPP::sendData(const QString &file, const QString &to) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::SEND_FILE;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        int length = to.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(to.toAscii(), length);

        length = file.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(file.toAscii(), length);

        m_ClientSocket->flush();
    }

    mutex.unlock();
}



// -------------------------------------------------------------
// room chat


void XMPP::createRoom(const QString& room) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::CREATE_ROOM;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        int length = room.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(room.toAscii(), length);

        m_ClientSocket->flush();
    }

    mutex.unlock();
}


void XMPP::addParticipant(const QString &participant) {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::ADD_PARTICIPANT;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));

        int length = participant.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(participant.toAscii(), length);

        m_ClientSocket->flush();
    }

    mutex.unlock();
}



// ------------------------------------------------------------
// Hub interface

void XMPP::updateHub() {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::UPDATE_HUB;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->flush();
    }

    mutex.unlock();
}

void XMPP::requestHubRemoval() {
    mutex.lockForWrite();

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::REMOVE_HUB;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->flush();
    }

    mutex.unlock();
}



// ------------------------------------------------------------
// failure area


bool XMPP::tryRestartHeadless() {
    qDebug() << "need restart";
    bb::system::InvokeRequest request;
    request.setTarget("com.amonchakai.Hg10Service");
    request.setAction("bb.action.START");
    bb::system::InvokeManager *invokeManager = new bb::system::InvokeManager(this);
    bb::system::InvokeTargetReply *reply = invokeManager->invoke(request);
    if (!reply) {
        qWarning() << "failed to start headless " << reply->errorCode();
        reply->deleteLater();
        invokeManager->deleteLater();

        return false;
    }

    return true;
}

void XMPP::closeCard() {
    emit closeCardRequest();
}


