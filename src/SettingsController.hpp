/*
 * AppSettings.hpp
 *
 *  Created on: 16 oct. 2014
 *      Author: pierre
 */

#ifndef APPSETTINGS_HPP_
#define APPSETTINGS_HPP_

#include <QtCore/QObject>
#include <QSettings>

class SettingsController : public QObject {
    Q_OBJECT;


    Q_PROPERTY( QString userName     READ getUserName        WRITE setUserName       NOTIFY  userNameChanged)
    Q_PROPERTY( QString avatar       READ getAvatar          WRITE setAvatar         NOTIFY  avatarChanged)
    Q_PROPERTY( int     theme        READ getTheme           WRITE setTheme          NOTIFY  themeChanged)
    Q_PROPERTY( int     fontSize     READ getFontSize        WRITE setFontSize       NOTIFY  fontSizeChanged)
    Q_PROPERTY( bool    enableGoogle READ getEnableGoogle    WRITE setEnableGoogle   NOTIFY  enableGoogleChanged)
    Q_PROPERTY( bool    googleLogged READ getGoogleLogged)
    Q_PROPERTY( bool    enableLogs   READ getLogEnabled      WRITE setLogEnabled     NOTIFY  logEnabledChanged)

private:

     QString            m_User;
     QString            m_Avatar;
     int                m_Theme;
     int                m_FontSize;
     bool               m_IsGoogleEnabled;
     static bool        m_LogEnabled;

     QSettings          *m_Settings;

public:
    SettingsController          (QObject *parent = 0);
    virtual ~SettingsController ()                      {};

    inline const QString &getUserName() const               { return m_User; }
    inline void           setUserName(const QString &c)     { m_User = c; emit userNameChanged(); }

    inline const QString &getAvatar() const                 { return m_Avatar; }
    inline void           setAvatar(const QString &c)       { m_Avatar = c; emit avatarChanged(); }

    inline int            getTheme() const                  { return m_Theme; }
    inline void           setTheme(int c)                   { m_Theme = c; emit themeChanged(); }

    inline int            getFontSize() const               { return m_FontSize; }
    inline void           setFontSize(int c)                { m_FontSize = c;  }

    inline bool           getEnableGoogle() const           { return m_IsGoogleEnabled; }
    inline void           setEnableGoogle(bool c)           { m_IsGoogleEnabled = c; emit enableGoogleChanged(); }

    inline bool           getLogEnabled() const             { return m_LogEnabled; }
    inline void           setLogEnabled(bool c)             { m_LogEnabled = c;  }
    static bool           isLogEnabled()                    { return m_LogEnabled; }

    bool                  getGoogleLogged();

public Q_SLOTS:
    void updateAvatar();
    void save();


Q_SIGNALS:

    void userNameChanged();
    void avatarChanged();
    void themeChanged();
    void fontSizeChanged();
    void enableGoogleChanged();
    void logEnabledChanged();

};


#endif /* APPSETTINGS_HPP_ */
