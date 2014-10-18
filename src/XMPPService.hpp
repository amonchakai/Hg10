/*
 * XMPPController.hpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */

#ifndef XMPPSERVICE_HPP_
#define XMPPSERVICE_HPP_


#include "client/QXmppClient.h"
#include "base/QXmppLogger.h"


class QXmppVCardIq;
class Contact;

class XMPP : public QXmppClient {
    Q_OBJECT;

public:
    virtual ~XMPP() {};

    static XMPP*                   get();
    inline const QList<Contact*>*  getContacts() const            {return m_Datas; }



private:
    static XMPP              *m_This;
    QList<Contact*>          *m_Datas;
    int                       m_WaitNbContacts;

    XMPP(QObject *parent = 0);


public Q_SLOTS:
    void messageReceived(const QXmppMessage&);
    void sendMessageTo(const QString &to, const QString &message);
    void presenceReceived(const QXmppPresence&);

    void rosterReceived();
    void vCardReceived(const QXmppVCardIq&);
    void loadvCard(const QString& bareJid);


Q_SIGNALS:
    void contactReceived();

};


#endif /* XMPPSERVICE_HPP_ */
