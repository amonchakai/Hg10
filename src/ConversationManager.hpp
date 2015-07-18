/*
 * ConversationManager.hpp
 *
 *  Created on: 13 oct. 2014
 *      Author: pierre
 */

#ifndef CONVERSATIONMANAGER_HPP_
#define CONVERSATIONMANAGER_HPP_

#include <QString>
#include "DataObjects.h"


class OnlineHistory;
class GoogleConnectController;


class ConversationManager : public QObject {
    Q_OBJECT;

public:
    virtual ~ConversationManager() {};

    static ConversationManager*    get();


    void initOnlineHistory              ();

    void clear                          ();
    void load                           (const QString &from, const QString &name);
    void reload                         ();
    void sendMessage                    (const QString &message);
    void sendMessage                    (const QString &to,   const QString &message);
    void logSent                        (const QString &to, const QString &message);
    void receiveMessage                 (const QString &from, const QString &to, const QString &message);

    inline const History&  getHistory   ()  const               { return m_History; };
    inline const QString&  getUser      ()  const               { return m_User; };
    void                   setUser      (const QString &n) ;
    inline const QString&  getAvatar    ()  const               { return m_Avatar; };
    inline const QString&  getAdressee  () const                { return m_CurrentDst; };
    inline bool            isAdressee   (const QString &who)    { return who.toLower() == m_CurrentDst.toLower(); }

    TimeEvent              getPreview   () const;
    TimeEvent              getPreview   (const QString &from) const;

    inline void            setAvatar    (const QString& a)      { m_Avatar = a; emit avatarUpdated();};

    void                   markRead     ();
    void                   markRead     (const QString &who);
    inline void            closeConversation()                  { m_CurrentDst = ""; m_BareID = ""; }
    void                   deleteHistory(const QString &with = "");

    void                   updateState  (const QString &who, int state);


    void                   sendData     (const QString &file);
    void                   loadUserName ();

    inline GoogleConnectController  *getFileTransfert()         { return m_FileTransfert; }


    void                    getPictureFromLink(const QString& user_id, const QString &picture_id);




    void onlineMessage                  (const QString &from, const QString &message, const QString &messageId);
    void flushHistory                   ();
    void saveHistory                    ();



private:
    static ConversationManager     *m_This;

    QString                         m_User;
    QString                         m_Avatar;
    QString                         m_CurrentDst;
    QString                         m_BareID;
    History                         m_History;
    QList<QString>                  m_GImageStack;
    int                             m_GStackIndex;

    // History from Google
    OnlineHistory                  *m_OnlineHistory;
    GoogleConnectController        *m_FileTransfert;
    int                             m_SynchPushLoc;



    ConversationManager                 (QObject *parent = 0);


public Q_SLOTS:
    void fowardImageReceived            (const QString& id, const QString& url);

Q_SIGNALS:
    void cleared                        ();
    void synchDone                      ();
    void historyLoaded                  ();
    void historyMessage                 (const QString &from, const QString &message);
    void messageReceived                (const QString &from, const QString &message);
    void messageSent                    (const QString &to,   const QString &message);
    void avatarUpdated                  ();
    void chatStateNotify                (int status);


    void imageURLFetched                (const QString& id, const QString& url);
};




#endif /* CONVERSATIONMANAGER_HPP_ */
