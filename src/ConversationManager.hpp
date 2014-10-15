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

class ConversationManager : public QObject {
    Q_OBJECT;

public:
    virtual ~ConversationManager() {};

    static ConversationManager*    get();

    void load           (const QString &from);
    void sendMessage    (const QString &message);
    void sendMessage    (const QString &to,   const QString &message);
    void receiveMessage (const QString &from, const QString &message);

    inline const History&  getHistory() const        { return m_History; };
    inline const QString&  getUser()    const        { return m_User; };
    inline const QString&  getAvatar()    const      { return m_Avatar; };

    TimeEvent              getPreview(const QString &from) const;

    inline void            setAvatar(const QString& a)  { m_Avatar = a; };


private:
    static ConversationManager     *m_This;

    QString                         m_User;
    QString                         m_Avatar;
    QString                         m_CurrentDst;
    QString                         m_BareID;
    History                         m_History;

    ConversationManager(QObject *parent = 0);
    void loadUserName();

public Q_SLOTS:


Q_SIGNALS:
    void historyLoaded();
    void messageReceived(const QString &from, const QString &message);

};




#endif /* CONVERSATIONMANAGER_HPP_ */
