/*
 * XMPPController.cpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */


#include "XMPPController.hpp"
#include "QXmppMessage.h"
#include <qDebug>

XMPP* XMPP::m_This = NULL;

XMPP::XMPP(QObject *parent) : QXmppClient(parent) {

    bool check = connect(this, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));

    Q_ASSERT(check);
    Q_UNUSED(check);

    check = connect(this, SIGNAL(presenceReceived(const QXmppPresence&)),
                        this, SLOT(presenceReceived(const QXmppPresence&)));
    Q_ASSERT(check);


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
