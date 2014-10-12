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


XMPP* XMPP::m_This = NULL;

XMPP::XMPP(QObject *parent) : QXmppClient(parent),
        m_Datas(new QList<Contact*>()) {

    bool check = connect(this, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));

    Q_ASSERT(check);

    check = connect(&this->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));
    Q_ASSERT(check);
    Q_UNUSED(check);

}


// get singleton
XMPP *XMPP::get() {
    if(m_This == NULL) {
        // if not already done, instantiate the network manager
        m_This = new XMPP(NULL);
    }

    return m_This;
}




void XMPP::messageReceived(const QXmppMessage& message) {
    qDebug() << message.from();
}

void XMPP::presenceReceived(const QXmppPresence& presence) {
    qDebug() << presence.from();
}

void XMPP::rosterReceived() {
    qDebug() << "example_9_vCard:: Roster Received";

    bool check = connect(&this->vCardManager(), SIGNAL(vCardReceived(QXmppVCardIq)), this, SLOT(vCardReceived(QXmppVCardIq)));

    Q_ASSERT(check);
    Q_UNUSED(check);

    QStringList list = rosterManager().getRosterBareJids();
    for(int i = 0; i < list.size(); ++i) {
        // request vCard of all the bareJids in roster
        vCardManager().requestVCard(list.at(i));
    }
}

void XMPP::vCardReceived(const QXmppVCardIq& vCard) {
    QString bareJid = vCard.from();

    QString out("FullName: %1");
    qDebug() << qPrintable(out.arg(vCard.fullName()));

    if(vCard.fullName().isEmpty())
        return;

    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");

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

    Contact *contact = new Contact;
    if(!photo.isEmpty()) {
        if(qImage.save(name)) {
            contact->setAvatar(vCardsDir + "/" + bareJid + ".png");
        } else contact->setAvatar("asset:///images/avatar.png");
    } else contact->setAvatar("asset:///images/avatar.png");


    contact->setName(vCard.fullName());
    contact->setID(bareJid);
    contact->setTimestamp("time");

    m_Datas->push_back(contact);

}

