/*
 * AppSettings.hpp
 *
 *  Created on: 16 oct. 2014
 *      Author: pierre
 */

#ifndef APPSETTINGS_HPP_
#define APPSETTINGS_HPP_

#include <QtCore/QObject>

class SettingsController : public QObject {
    Q_OBJECT;


    Q_PROPERTY( QString userName    READ getUserName    WRITE setUserName    NOTIFY userNameChanged)
    Q_PROPERTY( QString avatar      READ getAvatar      WRITE setAvatar      NOTIFY avatarChanged)
    Q_PROPERTY( int     theme       READ getTheme       WRITE setTheme      NOTIFY  themeChanged)

private:

     QString            m_User;
     QString            m_Avatar;
     int                m_Theme;


public:
    SettingsController(QObject *parent = 0);


    inline const QString &getUserName() const                 { return m_User; }
    inline void           setUserName(const QString &c)       { m_User = c; emit userNameChanged(); }

    inline const QString &getAvatar() const                 { return m_Avatar; }
    inline void           setAvatar(const QString &c)       { m_Avatar = c; emit avatarChanged(); }

    inline int            getTheme() const                 { return m_Theme; }
    inline void           setTheme(int c)                   { m_Theme = c; emit themeChanged(); }


public Q_SLOTS:
    void updateAvatar();


Q_SIGNALS:

    void userNameChanged();
    void avatarChanged();
    void themeChanged();

};


#endif /* APPSETTINGS_HPP_ */
