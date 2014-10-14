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


public:
    ConversationController(QObject *parent = 0);


public Q_SLOTS:
    inline void setWebView    (QObject *webView)      {m_WebView = dynamic_cast<bb::cascades::WebView*>(webView); }
    void updateView();
    void load(const QString &id);

    void send(const QString &message);
    void pushMessage(const QString &from, const QString &message);

Q_SIGNALS:
    void complete();


};


#endif /* CONVERSATIONCONTROLLER_HPP_ */
