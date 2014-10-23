/*
 * ConversationController.hpp
 *
 *  Created on: 13 oct. 2014
 *      Author: pierre
 */

#ifndef CONVERSATIONCONTROLLER_HPP_
#define CONVERSATIONCONTROLLER_HPP_


#include <QtCore/QObject>
#include <bb/cascades/WebView>


class ConversationController : public QObject {
    Q_OBJECT;

private:
    bb::cascades::WebView    *m_WebView;
    QString                  m_DstAvatar;
    bool                     m_HistoryCleared;


public:
    ConversationController              (QObject *parent = 0);



private:
    bool isImage                        (const QString &url);




public Q_SLOTS:
    inline void setWebView              (QObject *webView)                          {m_WebView = dynamic_cast<bb::cascades::WebView*>(webView); }
    void updateView                     ();
    void load                           (const QString &id, const QString &avatar);



    void send                           (const QString &message);
    void sendData                       (const QString &message);
    void pushMessage                    (const QString &from, const QString &message);
    void pushHistory                    (QString from, QString message);


    QString renderMessage               (const QString &message, bool showImg = true);


    void chatStateUpdate                (int status);




Q_SIGNALS:
    void complete();


};


#endif /* CONVERSATIONCONTROLLER_HPP_ */
