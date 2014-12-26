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
#include "OnlineHistory.hpp"
#include "FileTransfert.hpp"

class DriveItem;

class GoogleConnectController : public OnlineHistory, public FileTransfert {
    Q_OBJECT;

    Q_PROPERTY( bool    gmailAccess       READ getGmailAccess       WRITE setGmailAccess)
    Q_PROPERTY( int     driveAccess       READ getDriveAccess       WRITE setDriveAccess)

private:

    bool                                 m_GmailAccess;
    int                                  m_DriveAccess;


    bb::cascades::WebView               *m_WebView;
    QSettings                           *m_Settings;

    QStringList                          m_MessagesID;
    QStringList                          m_ThreadsID;
    QStringList                          m_Messages;
    QList<int>                           m_HistoryID;
    QList<int>                           m_IdxMessageToPush;
    QStringList                          m_Froms;

    QString                              m_LastThread;
    int                                  m_HistoryIndex;
    int                                  m_NBMessageExpected;
    QString                              m_LastSynchId;

    bool                                 m_StopListing;

    QString                              m_WithButNoKey;
    QString                              m_LastUploadedFile;
    QString                              m_DistUrl;
    QString                              m_ParentID;
    bool                                 m_CreatingRootDir;
    QString                              m_CurrentDir;

public:
     GoogleConnectController            (QObject *parent = 0);
     virtual ~GoogleConnectController   ()                      {};


     // -------------------------------------------------------------------------
     // oauth

     void getToken              ();
     void renewToken            ();
     void parse                 (const QString &message);
     void parseRefresh          (const QString &message);
     void getUserInfo           ();

     inline bool getGmailAccess() const         { return m_GmailAccess; };
     inline void setGmailAccess(bool c)         { m_GmailAccess = c;    };

     inline int  getDriveAccess() const         { return m_DriveAccess; };
     inline void setDriveAccess(int c)          { m_DriveAccess = c;    };

     // -------------------------------------------------------------------------
     // google drive

     void createFolder          (const QString &name = "Hg10", const QString &root = "");
     virtual void putFile       (const QString &path);
     virtual void share         ();
     void shareId               (const QString &id, const QString &path = "");
     void popFolder             ();
     void refresh               ();
     void setHomeFolder         (const QString &id);

     void parseFileList         (const QString &page);
     void parseFileEntry        (const QString &entry);


private:
     void checkOrder            (bool flush = false);
     void cleanupMessage        (QString &message);
     QString getContentTypeByExtension(const QString &extension);

public Q_SLOTS:

    // -------------------------------------------------------------------------
    // oauth login


    void logInRequest           ();
    void save                   (const QString &key);
    void checkReply             ();
    void checkRefresh           ();
    bool isLogged               ();



    // -------------------------------------------------------------------------
    // gmail


    virtual void getMessages            (const QString &with, int nbMessages);
    virtual void getRemainingMessages   (QString lastMessageId);

    void getMessageList         ();
    void getMessageReply        ();
    void replyGetUserInfo       ();





    // -------------------------------------------------------------------------
    // google drive

    void checkUploadReply       ();
    void uploading              (qint64 status, qint64 total);
    void checkCreateHomeReply   ();
    void checkReplyShare        ();

    void getFileList            (const QString &directory = "");
    void getFileListReply       ();

    // -------------------------------------------------------------------------
    // interface


    void setWebView             (QObject *webView);
    void webviewTitleChanged    (const QString &title);

Q_SIGNALS:

    void closeConnect();

    void contactInfoObtained    ();
    void uploading              (int status);
    void uploaded               ();
    void shared                 (const QString &url);
    void driveItemLoaded        (DriveItem *);
    void folderEmpty            ();
    void folderCreated          ();

};




#endif /* GOOGLECONNECTCONTROLLER_HPP_ */
