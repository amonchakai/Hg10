/*
 * ApplicationLogController.hpp
 *
 *  Created on: 1 déc. 2014
 *      Author: pierre
 */

#ifndef APPLICATIONLOGCONTROLLER_HPP_
#define APPLICATIONLOGCONTROLLER_HPP_


#include <QtCore/QObject>
#include <bb/cascades/WebView>


class ApplicationLogController : public QObject {
    Q_OBJECT;

private:
    bb::cascades::WebView               *m_WebView;


public:
    ApplicationLogController          (QObject *parent = 0);
    virtual ~ApplicationLogController ()                      {};




public Q_SLOTS:
    inline void setWebView             (QObject *webView)           {m_WebView = dynamic_cast<bb::cascades::WebView*>(webView); };
    void        showLogs               ();
    void        clearLogs              ();

Q_SIGNALS:


};



#endif /* APPLICATIONLOGCONTROLLER_HPP_ */
