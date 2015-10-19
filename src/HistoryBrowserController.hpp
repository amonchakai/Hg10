/*
 * HistoryBrowserController.hpp
 *
 *  Created on: 18 oct. 2015
 *      Author: pierre
 */

#ifndef HISTORYBROWSERCONTROLLER_HPP_
#define HISTORYBROWSERCONTROLLER_HPP_

#include <bb/cascades/ListView>
#include <bb/cascades/WebView>

class GoogleConnectController;

class HistoryBrowserController : public QObject {

    Q_OBJECT


private:

    bb::cascades::ListView           *m_HistoryList;
    GoogleConnectController          *m_GoogleConnect;
    bb::cascades::WebView            *m_WebView;


public:
    HistoryBrowserController(QObject *parent = 0);
    virtual ~HistoryBrowserController() {}

private:
    bool isOwnMessage(const QString &from);
    bool isImage     (const QString &url);
    QString renderMessage(const QString &message, bool showImg = true);


public Q_SLOTS:

    inline void setListView        (QObject *list)                 { m_HistoryList = dynamic_cast<bb::cascades::ListView*>(list);};
    inline void setWebView         (QObject *view)                 { m_WebView = dynamic_cast<bb::cascades::WebView*>(view);};

    void getThreads                (const QString& id, const QString& name, const QString &question);
    void loadThread                (const QString& id);



    void updateThreadsView         (const QByteArray& datas);
    void updateThread              (const QByteArray& datas);



// ----------------------------------------------------------------------------------------------
    Q_SIGNALS:

    void completed();
    void threadLoaded();
    void wallpaperChanged(const QString& wallpaper);
    void colorSet(const QString& color);


};


#endif /* HISTORYBROWSERCONTROLLER_HPP_ */
