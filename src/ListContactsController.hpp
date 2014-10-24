/*
 * ListContactsController.hpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */

#ifndef LISTCONTACTSCONTROLLER_HPP_
#define LISTCONTACTSCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <bb/platform/Notification>
#include <bb/cascades/ActivityIndicator>

class Contact;


class ListContactsController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( QString userName    READ getUserName    WRITE setUserName    NOTIFY userNameChanged)
    Q_PROPERTY( QString avatar      READ getAvatar      WRITE setAvatar      NOTIFY avatarChanged)


private:
    bb::cascades::ListView          *m_ListView;
    bb::cascades::ActivityIndicator *m_Activity;
    QString                          m_User;
    QString                          m_Avatar;
    QList<Contact *>                 m_Contacts;
    bool                             m_OnlyFavorite;

    bb::platform::Notification      *m_Notification;



public:
    ListContactsController              (QObject *parent = 0);


    inline const QString &getUserName   () const               { return m_User; }
    inline void           setUserName   (const QString &c)     { m_User = c; emit userNameChanged(); }


    inline const QString &getAvatar     () const               { return m_Avatar; }
    inline void           setAvatar     (const QString &c)     { m_Avatar = c; emit avatarChanged(); }





public Q_SLOTS:
    inline void setListView             (QObject *listView)    {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
    inline void setActivityIndicator    (QObject *activity)    {m_Activity = dynamic_cast<bb::cascades::ActivityIndicator*>(activity); m_Activity->start();}
    void updateView                     ();



    void updatePresence                 (const QString &who, int status);
    void messageReceived                (const QString &from, const QString &message);
    void markRead                       ();

    QString formatTime                  (qint64 msecs);

    inline void setFilter               (bool onlyFav)         { m_OnlyFavorite = onlyFav; }
    void updateConnectionStatus         (bool status);

    void clear                          ();


Q_SIGNALS:
    void complete                       ();
    void cleared                        ();

    void userNameChanged                ();
    void avatarChanged                  ();


};



#endif /* LISTCONTACTSCONTROLLER_HPP_ */
