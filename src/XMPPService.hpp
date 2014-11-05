/*
 * XMPPController.hpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */

#ifndef XMPPSERVICE_HPP_
#define XMPPSERVICE_HPP_

#include <QtNetwork/QTcpSocket>

class Contact;
class Facebook;

class XMPP : public QObject {
    Q_OBJECT;

public:
    virtual ~XMPP() {};

    static XMPP*                   get();
    const QList<Contact*>*         getContacts();

    void                           connectToServer     (const QString &user, const QString &password);
    void                           disconnectFromServer();


private:
    static XMPP              *m_This;
    QList<Contact*>          *m_Datas;
    QMap<QString, Contact*>  *m_PushStack;
    bool                      m_Connected;
    Facebook                 *m_Facebook;

    // ------------------------------------------------------------
    // connection to headless XMPP service.
    QTcpSocket               *m_ClientSocket;
    bool                      m_ScheduleContactListRequest;

    XMPP(QObject *parent = 0);

    void loadLocal          ();

public Q_SLOTS:

    void getContactList     ();
    void sendMessageTo      (const QString &to, const QString &message);

    void loadvCard          (const QString& bareJid, bool push = false);




    // -------------------------------------------------------------
    // file transfer handling

    void sendData           (const QString &file, const QString &to);





    // -------------------------------------------------------------
    // cleanup

    void clear              ();




    // -------------------------------------------------------------
    // chat rooms

    void createRoom             (const QString &roomName);
    void addParticipant         (const QString &participant);



    // -------------------------------------------------------------
    // aside API

    void initFacebook           ();
    void facebookImagesRetrieved(const QString &who);






    // ------------------------------------------------------------
    // connection to headless XMPP service.

    void connected           ();
    void connectionToServiceFailed(QAbstractSocket::SocketError e);
    void disconnected        ();
    void readyRead           ();
    void connectToXMPPService();

    void notifySettingChange ();





    // ------------------------------------------------------------
    // failure area

    bool tryRestartHeadless ();

Q_SIGNALS:

    void offline            (bool status);
    void contactReceived    ();
    void pushContact        (const Contact*);
    void presenceUpdated    (const QString &who, int status);

    void connectedXMPP       ();
    void connectionFailed    ();
};


#endif /* XMPPSERVICE_HPP_ */
