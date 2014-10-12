/*
 * XMPPController.hpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */

#ifndef XMPPCONTROLLER_HPP_
#define XMPPCONTROLLER_HPP_


#include "client/QXmppClient.h"
#include "base/QXmppLogger.h"

class XMPP : public QXmppClient {
    Q_OBJECT;

public:
    virtual ~XMPP() {};

    static XMPP* get();


private:
    static XMPP     *m_This;

    XMPP(QObject *parent = 0);


public Q_SLOTS:
    void messageReceived(const QXmppMessage&);
    void presenceReceived(const QXmppPresence&);


};


#endif /* XMPPCONTROLLER_HPP_ */
