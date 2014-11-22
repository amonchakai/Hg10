/*
 * Facebook.hpp
 *
 *  Created on: 27 oct. 2014
 *      Author: pierre
 */

#ifndef FACEBOOK_HPP_
#define FACEBOOK_HPP_

#include <QtCore/QObject>
#include <QMap>
#include <bb/cascades/WebView>
#include <QSettings>
#include "OnlineHistory.hpp"

class Facebook : public OnlineHistory {
    Q_OBJECT;

    QMap<QString, QString>      m_UrlToUser;


private:
    bb::cascades::WebView               *m_WebView;
    QSettings                           *m_Settings;

    QString                              m_CurrentDst;
    QString                              m_CurrentDstId;
    int                                  m_NbMessages;

    QStringList                          m_Messages;
    QStringList                          m_Who;
    QStringList                          m_When;
    int                                  m_Pos;


    void    getInbox    ();
    void    parseInbox  (const QString &page);

public:
    Facebook            (QObject *parent = 0);
    virtual ~Facebook   ()                      {};


    void getAvatar      (const QString &user_id);
    void downloadImage  (const QString url);
    void saveImages     (const QByteArray&, const QString &username);


    virtual void getMessages            (const QString &with, int nbMessages);
    virtual void getRemainingMessages   (QString lastMessageId);


public Q_SLOTS:
    void checkReplyRedirectUrl  ();
    void checkDownload          ();

    void setWebView             (QObject *webView);


    void logInRequest           ();
    void navigationRequested    (bb::cascades::WebNavigationRequest *);

    void checkReplyUserID       ();
    void checkReplyUserName     ();
    void checkReplyInbox        ();
    void sendProgressively      ();


Q_SIGNALS:
    void imagesRetrieved        (const QString&);
    void contactInfoObtained    ();


    void closeConnect           ();
    void messageLoaded          (const QString &from, const QString &what, const QString &messageId);
    void synchCompleted         ();
};


#endif /* FACEBOOK_HPP_ */
