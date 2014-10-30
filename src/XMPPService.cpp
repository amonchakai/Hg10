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
#include <bb/cascades/ImageTracker>
#include <QtNetwork/QTcpSocket>

#include "DataObjects.h"
#include <QReadWriteLock>
#include "ConversationManager.hpp"


QReadWriteLock  mutex;
QReadWriteLock  mutexLoadLocal;

XMPP* XMPP::m_This = NULL;

XMPP::XMPP(QObject *parent) : QObject(parent),
        m_Datas(new QList<Contact*>()),
        m_Connected(false),
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
        int code = XMPPServiceMessages::REQUEST_CONTACT_LIST;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->flush();
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
    int code = *reinterpret_cast<int*>(code_str.data());
    qDebug() << "COMMAND: " << code;

    switch(code) {

        case XMPPServiceMessages::REPLY_CONTACT_LIST: {
            qDebug() << "list received";
            code_str = m_ClientSocket->read(sizeof(int));
            int nbCards = *reinterpret_cast<int*>(code_str.data());
            mutex.lockForWrite();
            m_WaitNbContacts = nbCards;
            m_Datas->clear();
            mutex.unlock();
            for(int i = 0 ; i < nbCards ; ++i) {
                code_str = m_ClientSocket->read(sizeof(int));
                int length = *reinterpret_cast<int*>(code_str.data());
                QString name(m_ClientSocket->read(length));

                loadvCard(name);

            }
            emit offline(false);

        }
            break;


        case XMPPServiceMessages::REPLY_LOGGED_IN: {
            emit connectedXMPP();
        }
            break;

    }

}

// ===========================================================================================


void XMPP::connectToServer(const QString &user, const QString& password) {
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

}

void XMPP::disconnectFromServer() {
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        int code = XMPPServiceMessages::DISCONNECT;
        m_ClientSocket->write(reinterpret_cast<char*>(&code), sizeof(int));
        m_ClientSocket->flush();
    }
}


// ===========================================================================================


void XMPP::getContactList() {
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        m_ClientSocket->write(QByteArray::number(XMPPServiceMessages::REQUEST_CONTACT_LIST));
        m_ClientSocket->flush();
    }
}


void XMPP::loadLocal() {
    mutexLoadLocal.lockForWrite();

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

}

void XMPP::clear() {
    m_Datas->clear();
    ConversationManager::get()->clear();
}

void XMPP::messageReceived() {
/*
    if(message.body().isEmpty())
        ConversationManager::get()->updateState(message.from(), message.state());
    else
        ConversationManager::get()->receiveMessage(message.from(), message.to(), message.body());
        */
}

void XMPP::presenceReceived(const QString &who, int status) {
    emit presenceUpdated(who, status);
}



void XMPP::loadvCard(const QString &bareJid) {
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

    QString photoName = bareJid;
    QRegExp isFacebook("(.*)@chat.facebook.com");
    if(isFacebook.indexIn(photoName) != -1) {
        if(photoName[0] == '-')
            photoName = photoName.mid(1);
    }


    if(QFile::exists(vCardsDir + "/" + photoName + ".png"))
        contact->setAvatar(vCardsDir + "/" + photoName + ".png");
    else
        contact->setAvatar("asset:///images/avatar.png");

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
    --m_WaitNbContacts;
    if(m_WaitNbContacts == 0)
        emit contactReceived();
    mutex.unlock();

}

void XMPP::sendMessageTo(const QString &to, const QString &message) {

}


// -------------------------------------------------------------
// file transfer handling

void XMPP::sendData(const QString &file, const QString &to) {

}





