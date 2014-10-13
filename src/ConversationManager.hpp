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
    void sendMessage    (const QString &to,   const QString &message);
    void receiveMessage (const QString &from, const QString &message);


private:
    static ConversationManager     *m_This;

    QString                         m_User;
    QString                         m_CurrentDst;
    History                         m_History;

    ConversationManager(QObject *parent = 0);
    void loadUserName();

public Q_SLOTS:


Q_SIGNALS:
    void historyLoaded();

};




#endif /* CONVERSATIONMANAGER_HPP_ */
