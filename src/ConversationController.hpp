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

class DropBoxConnectController;

class ConversationController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( bool    isRoom    READ getRoom       WRITE setRoom       NOTIFY roomChanged)

private:
    bb::cascades::WebView    *m_WebView;
    QString                   m_DstAvatar;
    bool                      m_HistoryCleared;
    bool                      m_IsRoom;
    DropBoxConnectController *m_DropboxController;




public:
    ConversationController              (QObject *parent = 0);
    virtual ~ConversationController     ()                      {};


    inline bool getRoom() const                                                     { return m_IsRoom; }
    inline void setRoom(bool r)                                                     { m_IsRoom = r; }

private:
    bool isImage                        (const QString &url);
    bool isOwnMessage                   (const QString &from);
    void initDropbox                    ();



public Q_SLOTS:
    inline void setWebView              (QObject *webView)                          {m_WebView = dynamic_cast<bb::cascades::WebView*>(webView); }
    void updateView                     ();
    void load                           (const QString &id, const QString &avatar);



    void send                           (const QString &message);
    void sendData                       (const QString &message);
    void pushMessage                    (const QString &from, const QString &message);
    void pushHistory                    (const QString &from, const QString &message);


    QString renderMessage               (const QString &message, bool showImg = true);


    void chatStateUpdate                (int status);


    // -----------------------------------------------------
    // messages with dropbox

    void uploaded                       ();
    void shared                         (const QString &url);
    void fowardUploadingProcess         (int progress);


Q_SIGNALS:
    void complete                       ();
    void uploading                      (int status);
    void receivedUrl                    (const QString &url);

    void roomChanged                    ();

};


#endif /* CONVERSATIONCONTROLLER_HPP_ */
