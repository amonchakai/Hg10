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
#include <QtNetwork/QTcpSocket>

#include "DataObjects.h"
#include "ConversationManager.hpp"
#include "Facebook.hpp"

QReadWriteLock  mutex;
QReadWriteLock  mutexLoadLocal;

XMPP* XMPP::m_This = NULL;

XMPP::XMPP(QObject *parent) : QObject(parent),
        m_Datas(new QList<Contact*>()),
        m_PushStack(NULL),
        m_Connected(false),
        m_Facebook(NULL),
        m_ClientSocket(new QTcpSocket(this)) {

    bool check = connect(m_ClientSocket, SIGNAL(connected()), this, SLOT(connected()));
    Q_ASSERT(check);
    Q_UNUSED(check);


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


void XMPP::connected() {

    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Connected to XMPP headless!" << m_ClientSocket->state() ;
        mutex.lockForWrite();
        m_Datas->clear();
        int code = XMPPServiceMessages::REQUEST_CONTACT_LIST;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->flush();
        mutex.unlock();
    }

}


void XMPP::connectToXMPPService() {
    if (!m_ClientSocket->isOpen()) {
        m_ClientSocket->connectToHost(QHostAddress::LocalHost, 2014);
        bool ok = connect(m_ClientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        Q_ASSERT(ok);
        ok = connect(m_ClientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
    } else {
        connected();
    }
}

void XMPP::disconnected() {

}

void XMPP::readyRead() {

    QByteArray code_str = m_ClientSocket->read(sizeof(int));
    while(code_str.size() == sizeof(int)) {
        int code = *reinterpret_cast<int*>(code_str.data());
        qDebug() << "COMMAND: " << code;

        switch(code) {

            case XMPPServiceMessages::REPLY_CONTACT_LIST: {
                qDebug() << "list received";
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString name(m_ClientSocket->read(length));

                loadvCard(name, true);

                emit offline(false);

            }
                break;


            case XMPPServiceMessages::REPLY_LOGGED_IN: {
                emit connectedXMPP();
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
                QString body(m_ClientSocket->read(length));

                ConversationManager::get()->receiveMessage(from, to, body);

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
        m_ClientSocket->write(QByteArray::number(XMPPServiceMessages::REQUEST_CONTACT_LIST));
        m_ClientSocket->flush();
    }
    mutex.unlock();
}


void XMPP::loadLocal() {
    mutexLoadLocal.lockForWrite();

    QRegExp isFacebook("(.*)@chat.facebook.com");
    bool delayPush = false;

    QString directory = QDir::homePath() + QLatin1String("/vCards");
    if (QFile::exists(directory)) {
        QDir dir(directory);
        dir.setNameFilters(QStringList() << "*.xml");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList()) {

            if(isFacebook.indexIn(dirFile.mid(0,dirFile.length()-4)) != -1)
                delayPush = true;

            //dir.remove(dirFile);
            //qDebug() << dirFile;
            loadvCard(dirFile.mid(0,dirFile.length()-4));
        }
    }

    mutexLoadLocal.unlock();

    if(!delayPush)
        emit contactReceived();

}

void XMPP::clear() {
    m_Datas->clear();
    ConversationManager::get()->clear();
}



void XMPP::loadvCard(const QString &bareJid, bool push) {
    // -------------------------------------------------------------
    // get vCard from file
    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");
    QFile file(vCardsDir + "/" + bareJid + ".xml");

    QDomDocument doc("vCard");
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();


    QXmppVCardIq vCard;
    vCard.parse(doc.documentElement());


    // --------------------------------------------------------------
    // insert vCard content to view

    Contact *contact = new Contact;
    contact->setID(bareJid);
    bool delayPush = false;

    QString photoName = bareJid;
    QRegExp isFacebook("(.*)@chat.facebook.com");
    if(isFacebook.indexIn(photoName) != -1) {
        photoName = isFacebook.cap(1);
        if(photoName[0] == '-')
            photoName = photoName.mid(1);

        push = true;

        if(m_Facebook == NULL)
            initFacebook();

        if(!QFile::exists(vCardsDir + "/" + photoName + "@chat.facebook.com.png")) {
            m_Facebook->getAvatar(photoName);
            delayPush = true;
            mutex.lockForWrite();
            if(m_PushStack == NULL) {
                m_PushStack = new QMap<QString, Contact*>();
            }
            m_PushStack->insert(photoName, contact);
            mutex.unlock();
        }

        contact->setAvatar(vCardsDir + "/" + photoName + "@chat.facebook.com.png");

    } else {

        if(QFile::exists(vCardsDir + "/" + photoName + ".png"))
            contact->setAvatar(vCardsDir + "/" + photoName + ".png");
        else
            contact->setAvatar("asset:///images/avatar.png");
    }

    contact->setName(vCard.fullName());
    contact->setTimestamp(0);

    if(vCard.fullName().isEmpty())
        contact->deleteLater();
    else {
        if(m_Connected) {
            contact->setPresence(0);

/*
            QStringList resources = rosterManager().getResources(bareJid);
            //qDebug() << "Resource size: " << resources.size();
            for(int i = 0 ; i < resources.size() ; ++i) {
                QXmppPresence presence = rosterManager().getPresence(bareJid,resources.at(i));

                //qDebug() << "Resource values: " << i << presence.availableStatusType();

                contact->setPresence(std::max<int>(contact->getPresence(), presence.availableStatusType()));
            }
*/

        } else
            contact->setPresence(0);
        m_Datas->push_back(contact);
    }

    mutex.lockForWrite();
    if(push && !vCard.fullName().isEmpty() && !delayPush)
        emit pushContact(contact);
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

        length = message.length();
        m_ClientSocket->write(reinterpret_cast<char*>(&length), sizeof(int));
        m_ClientSocket->write(message.toAscii(), length);

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
// aside API

void XMPP::initFacebook() {
    m_Facebook = new Facebook(this);

    bool check = connect(m_Facebook, SIGNAL(imagesRetrieved(const QString&)), this, SLOT(facebookImagesRetrieved(const QString&)));
    Q_ASSERT(check);
}

void XMPP::facebookImagesRetrieved(const QString &who) {
    qDebug() << "id from facebook: " <<who;

    mutex.lockForWrite();
    Contact *c = m_PushStack->value(who);
    emit pushContact(c);
    mutex.unlock();
}





