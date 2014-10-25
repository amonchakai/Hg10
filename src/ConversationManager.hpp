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

class GoogleConnectController;

enum GoogleSynchStatus {
    NONE  = 0,
    PUSH  = 1,
    FLUSH = 2
};

class ConversationManager : public QObject {
    Q_OBJECT;

public:
    virtual ~ConversationManager() {};

    static ConversationManager*    get();

    void clear                          ();
    void load                           (const QString &from);
    void sendMessage                    (const QString &message);
    void sendMessage                    (const QString &to,   const QString &message);
    void logSent                        (const QString &to, const QString &message);
    void receiveMessage                 (const QString &from, const QString &to, const QString &message);

    inline const History&  getHistory   ()  const               { return m_History; };
    inline const QString&  getUser      ()  const               { return m_User; };
    inline const QString&  getAvatar    ()  const               { return m_Avatar; };
    inline bool            isAdressee   (const QString &who)    { return who.toLower() == m_CurrentDst.toLower(); }

    TimeEvent              getPreview   () const;
    TimeEvent              getPreview   (const QString &from) const;

    inline void            setAvatar    (const QString& a)      { m_Avatar = a; emit avatarUpdated();};

    void                   markRead     ();
    inline void            closeConversation()                  { m_CurrentDst = ""; m_BareID = ""; }

    void                   updateState  (const QString &who, int state);


    void                   sendData     (const QString &file);
    void                   loadUserName ();

private:
    static ConversationManager     *m_This;

    QString                         m_User;
    QString                         m_Avatar;
    QString                         m_CurrentDst;
    QString                         m_BareID;
    History                         m_History;

    // History from Google
    GoogleConnectController        *m_GoogleConnect;
    GoogleSynchStatus               m_SynchStatus;
    int                             m_SynchPushLoc;




    ConversationManager                 (QObject *parent = 0);

public Q_SLOTS:
    void googleMessage                  (const QString &from, const QString &message, const QString &messageId);
    void saveHistory                    ();

Q_SIGNALS:
    void cleared                        ();
    void historyLoaded                  ();
    void historyMessage                 (const QString &from, const QString &message);
    void messageReceived                (const QString &from, const QString &message);
    void messageSent                    (const QString &to,   const QString &message);
    void avatarUpdated                  ();
    void chatStateNotify                (int status);

};




#endif /* CONVERSATIONMANAGER_HPP_ */
