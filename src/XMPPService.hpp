/*
 * XMPPController.hpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */

#ifndef XMPPSERVICE_HPP_
#define XMPPSERVICE_HPP_


class QTcpSocket;
class Contact;

class XMPP : public QObject {
    Q_OBJECT;

public:
    virtual ~XMPP() {};

    static XMPP*                   get();
    inline const QList<Contact*>*  getContacts() const            {return m_Datas; }

    void                           connectToServer     (const QString &user, const QString &password);
    void                           disconnectFromServer();


private:
    static XMPP              *m_This;
    QList<Contact*>          *m_Datas;
    int                       m_WaitNbContacts;
    bool                      m_Connected;


    // ------------------------------------------------------------
    // connection to headless XMPP service.
    QTcpSocket               *m_ClientSocket;

    XMPP(QObject *parent = 0);

    void loadLocal          ();

public Q_SLOTS:

    void getContactList     ();
    void messageReceived    ();
    void sendMessageTo      (const QString &to, const QString &message);
    void presenceReceived   (const QString &who, int status);

    void loadvCard          (const QString& bareJid);

    // -------------------------------------------------------------
    // file transfer handling

    void sendData           (const QString &file, const QString &to);

    // -------------------------------------------------------------
    // cleanup

    void clear              ();


    // ------------------------------------------------------------
    // connection to headless XMPP service.

    void connected           ();
    void disconnected        ();
    void readyRead           ();
    void connectToXMPPService();

Q_SIGNALS:

    void offline            (bool status);
    void contactReceived    ();
    void presenceUpdated    (const QString &who, int status);

    void connectedXMPP       ();
};


#endif /* XMPPSERVICE_HPP_ */
