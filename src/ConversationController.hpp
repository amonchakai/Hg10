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
#include <bb/cascades/ActivityIndicator>
#include <bb/system/SystemUiResult>
#include "DataObjects.h"

class FileTransfert;

class ConversationController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( bool    isRoom         READ getRoom       WRITE setRoom       NOTIFY roomChanged)
    Q_PROPERTY( QString nextAudioFile  READ getNextAudioName)
    Q_PROPERTY( bool    fileReady      READ getFileReady)

private:
    bb::cascades::WebView    *m_WebView;
    bb::cascades::ActivityIndicator *m_LinkActivity;
    QString                   m_DstAvatar;
    bool                      m_HistoryCleared;
    bool                      m_IsRoom;
    bool                      m_UploadingAudio;
    QString                   m_AudioFileName;
    FileTransfert            *m_FileTransfert;
    QList<AudioMessage>       m_AudioMessages;

    QString                   m_NewWallpaper;



public:
    ConversationController              (QObject *parent = 0);
    virtual ~ConversationController     ()                      {};


    inline bool getRoom() const                                                     { return m_IsRoom; }
    inline void setRoom(bool r)                                                     { m_IsRoom = r; }
    inline bool getFileReady()                                                      { return m_UploadingAudio; }

private:
    bool isImage                        (const QString &url);
    bool isOwnMessage                   (const QString &from);
    void initDropbox                    ();
    void initGoogleDrive                ();



public Q_SLOTS:
    inline void setWebView              (QObject *webView)                          {m_WebView = dynamic_cast<bb::cascades::WebView*>(webView); }
    inline void setLinkActivity         (QObject *activity)                         {m_LinkActivity = dynamic_cast<bb::cascades::ActivityIndicator*>(activity);};
    void updateView                     ();
    void load                           (const QString &id, const QString &avatar, const QString &name);
    void refreshHistory                 (const QString &id, const QString &avatar, const QString &name);

    void linkEstablished                ();
    void waitingLink                    ();

    void send                           (const QString &message);
    void sendAudioData                  (const QString &message);
    void sendData                       (const QString &message);
    void pushMessage                    (const QString &from, const QString &message);
    void pushHistory                    (const QString &from, const QString &message);


    QString renderMessage               (const QString &message, bool showImg = true);


    void chatStateUpdate                (int status);


    // -----------------------------------------------------
    // audio messages


    QString getNextAudioName            ();

    // -----------------------------------------------------
    // messages with dropbox

    void uploaded                       ();
    void shared                         (const QString &url);
    void fowardUploadingProcess         (int progress);


    // -----------------------------------------------------
    // aesthetic aspects.

    void setWallpaper                   ();
    void onWallpaperSelected            (const QStringList&);
    void onWallpaperSelectCanceled      ();
    void onPromptFinishedSetWallpaper   (bb::system::SystemUiResult::Type);


    // -----------------------------------------------------
    // messages with Hub

    void closeCard                      ();

Q_SIGNALS:
    void complete                       ();
    void uploading                      (int status);
    void receivedUrl                    (const QString &url);

    void roomChanged                    ();
    void wallpaperChanged               (const QString &url);

};


#endif /* CONVERSATIONCONTROLLER_HPP_ */
