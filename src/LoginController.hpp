/*
 * XMPPController.hpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */

#ifndef LOGINCONTROLLER_HPP_
#define LOGINCONTROLLER_HPP_


#include "XMPPService.hpp"

class LoginController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( bool    notif       READ getNotif       WRITE setNotif       NOTIFY notifChanged)

private:
    QString m_User;
    QString m_Password;
    bool    m_Notif;

public:
    LoginController             (QObject *parent = 0);
    virtual ~LoginController    ()                      {};

    inline const bool     getNotif      () const               { return m_Notif; }
    void                  setNotif      (bool c);


public Q_SLOTS:

    void login                  (const QString& login, const QString& password);
    void logOut                 ();
    bool isLogged               ();
    void deleteHistory          ();
    void clearContactsData      ();
    void connected              ();
    void connectionFailed       ();


Q_SIGNALS:
    void complete               ();
    void connectError           ();
    void notifChanged           ();


private:
    void saveUserName           ();
    void loadUserName           ();

};


#endif /* LOGINCONTROLLER_HPP_ */
