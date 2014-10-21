/*
 * GoogleConnectController.hpp
 *
 *  Created on: 19 oct. 2014
 *      Author: pierre
 */

#ifndef GOOGLECONNECTCONTROLLER_HPP_
#define GOOGLECONNECTCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/WebView>
#include <QSettings>

class GoogleConnectController : public QObject {
    Q_OBJECT;


private:
    bb::cascades::WebView               *m_WebView;
    QSettings                           *m_Settings;

    QStringList                          m_MessagesID;
    QStringList                          m_ThreadsID;
    QString                              m_LastThread;
    int                                  m_HistoryIndex;
    int                                  m_NBMessageExpected;
    QString                              m_LastSynchId;

    bool                                 m_StopListing;

public:
     GoogleConnectController    (QObject *parent = 0);

     void getToken              ();
     void renewToken            ();
     void parse                 (const QString &message);
     void parseRefresh          (const QString &message);

public Q_SLOTS:
    void logInRequest           ();
    void save                   (const QString &key);
    void checkReply             ();
    void checkRefresh           ();

    void getMessages            (const QString &with, int nbMessages);
    void getRemainingMessages   (QString lastMessageId);

    void getMessageList         ();
    void getMessageReply        ();


    inline void setWebView      (QObject *webView)                {m_WebView = dynamic_cast<bb::cascades::WebView*>(webView); }

Q_SIGNALS:

    void messageLoaded          (QString from, QString what, QString messageId);
    void synchCompleted         ();

};




#endif /* GOOGLECONNECTCONTROLLER_HPP_ */
