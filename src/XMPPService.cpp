/*
 * XMPPController.cpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */


#include "XMPPService.hpp"
#include "QXmppMessage.h"
#include "client/QXmppRosterManager.h"
#include "QXmppVCardIq.h"
#include "client/QXmppVCardManager.h"

#include <QDir>
#include <QFile>
#include <QBuffer>
#include <QDebug>

#include <bb/cascades/ImageTracker>

#include "DataObjects.h"
#include <QReadWriteLock>
#include "ConversationManager.hpp"

QReadWriteLock  mutex;
QReadWriteLock  mutexLoadLocal;

XMPP* XMPP::m_This = NULL;

XMPP::XMPP(QObject *parent) : QXmppClient(parent),
        m_Datas(new QList<Contact*>()),
        m_WaitNbContacts(0),
        m_TransferManager(NULL) {

    bool check = connect(this, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));

    Q_ASSERT(check);

    check = connect(&this->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    m_TransferManager = new QXmppTransferManager();
    m_TransferManager->setProxy("proxy.eu.jabber.org");
    addExtension(m_TransferManager);

    check = connect(m_TransferManager, SIGNAL(fileReceived(QXmppTransferJob*)), this, SLOT(fileReceived(QXmppTransferJob*)));
    Q_ASSERT(check);

    loadLocal();

}


// get singleton
XMPP *XMPP::get() {
    if(m_This == NULL) {
        // if not already done, instantiate the network manager
        m_This = new XMPP(NULL);
    }

    return m_This;
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
            qDebug() << dirFile;
            loadvCard(dirFile.mid(0,dirFile.length()-4));
        }
    }

    mutexLoadLocal.unlock();

}


void XMPP::messageReceived(const QXmppMessage& message) {
    if(message.body().isEmpty())
        ConversationManager::get()->updateState(message.from(), message.state());
    else
        ConversationManager::get()->receiveMessage(message.from(), message.to(), message.body());
}

void XMPP::presenceReceived(const QXmppPresence& presence) {
    emit presenceUpdated(presence.from(), presence.availableStatusType());
}

void XMPP::rosterReceived() {
    mutexLoadLocal.lockForWrite();
    m_Datas->clear();

    ConversationManager::get()->getUser();

    bool check = connect(&this->vCardManager(), SIGNAL(vCardReceived(QXmppVCardIq)), this, SLOT(vCardReceived(QXmppVCardIq)));

    Q_ASSERT(check);
    Q_UNUSED(check);

    QStringList list = rosterManager().getRosterBareJids();



    mutex.lockForWrite();
    m_WaitNbContacts = list.size()+1;
    mutex.unlock();

    vCardManager().requestClientVCard();

    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");

    for(int i = 0; i < list.size(); ++i) {
        // request vCard of all the bareJids in roster
        if(QFile::exists(vCardsDir + "/" + list.at(i) + ".xml")) {
            loadvCard(list.at(i));
        } else {
            vCardManager().requestVCard(list.at(i));
        }
    }

    mutexLoadLocal.unlock();
    emit offline(false);
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

    if(QFile::exists(vCardsDir + "/" + bareJid + ".png"))
        contact->setAvatar(vCardsDir + "/" + bareJid + ".png");
    else
        contact->setAvatar("asset:///images/avatar.png");

    contact->setName(vCard.fullName());
    contact->setTimestamp(0);

    if(vCard.fullName().isEmpty())
        contact->deleteLater();
    else {
        QStringList resources = rosterManager().getResources(bareJid);
        for(int i = 0 ; i < resources.size() ; ++i) {
            QXmppPresence presence = rosterManager().getPresence(bareJid,resources.at(i));
            contact->setPresence(presence.availableStatusType());
        }
        m_Datas->push_back(contact);
    }

    mutex.lockForWrite();
    --m_WaitNbContacts;
    if(m_WaitNbContacts == 0)
        emit contactReceived();
    mutex.unlock();

}

void XMPP::vCardReceived(const QXmppVCardIq& vCard) {
    QString bareJid = vCard.from();
    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");
    bool ownInfo = false;

    Contact *contact = new Contact;
    if(bareJid.isEmpty()) {
        bareJid = ConversationManager::get()->getUser();
        ownInfo = true;
    }
    contact->setID(bareJid);


    QDir dir;
    if(!dir.exists(vCardsDir))
        dir.mkdir(vCardsDir);

    QFile file(vCardsDir + "/" + bareJid + ".xml");
    if(file.open(QIODevice::ReadWrite))
    {
        QXmlStreamWriter stream(&file);
        vCard.toXml(&stream);
        file.close();
    }

    QString name(vCardsDir + "/" + bareJid + ".png");
    QByteArray photo = vCard.photo();
    QImage qImage;
    qImage.loadFromData(vCard.photo());

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

    if(!photo.isEmpty()) {
        if(qImage.convertToFormat(QImage::Format_ARGB4444_Premultiplied).save(name, "PNG")) {
            contact->setAvatar(vCardsDir + "/" + bareJid + ".png");
        } else contact->setAvatar("asset:///images/avatar.png");
    } else contact->setAvatar("asset:///images/avatar.png");


    if(ownInfo)
        ConversationManager::get()->setAvatar(contact->getAvatar());

    contact->setName(vCard.fullName());
    contact->setTimestamp(0);

    if(vCard.fullName().isEmpty())
        contact->deleteLater();
    else {
        QStringList resources = rosterManager().getResources(bareJid);
        for(int i = 0 ; i < resources.size() ; ++i) {
            QXmppPresence presence = rosterManager().getPresence(bareJid,resources.at(i));
            contact->setPresence(presence.availableStatusType());
        }
        m_Datas->push_back(contact);
    }

    mutex.lockForWrite();
    --m_WaitNbContacts;
    if(m_WaitNbContacts == 0)
        emit contactReceived();
    mutex.unlock();

}

void XMPP::sendMessageTo(const QString &to, const QString &message) {

    sendPacket(QXmppMessage("", to, message));
}


// -------------------------------------------------------------
// file transfer handling

void XMPP::sendData(const QString &file, const QString &to) {
    qDebug() << "Send file request... " << file << to;
    QXmppTransferJob *job = m_TransferManager->sendFile(to, file, "file sent from Hg10");

    bool check = connect(job, SIGNAL(error(QXmppTransferJob::Error)),
                    this, SLOT(transferError(QXmppTransferJob::Error)));
    Q_ASSERT(check);

    check = connect(job, SIGNAL(finished()),
                    this, SLOT(transferFinished()));
    Q_ASSERT(check);

    check = connect(job, SIGNAL(progress(qint64,qint64)),
                    this, SLOT(transferInProgress(qint64,qint64)));
    Q_ASSERT(check);

}


void XMPP::fileReceived(QXmppTransferJob* job) {

}

void XMPP::transferError(QXmppTransferJob::Error error) {
    qDebug() << "Transmission failed:" << error;
}

void XMPP::transferFinished() {
    qDebug() << "Transmission finished";
}

void XMPP::transferInProgress(qint64 done,qint64 total) {
    qDebug() << "Transmission progress:" << done << "/" << total;
}
