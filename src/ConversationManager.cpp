/*
 * ConversationManager.cpp
 *
 *  Created on: 13 oct. 2014
 *      Author: pierre
 */


#include "ConversationManager.hpp"

#include <QReadWriteLock>
#include "DataObjects.h"
#include <QDate>
#include <QFile>
#include <QDir>
#include <QDebug>

#include "XMPPService.hpp"
#include "GoogleConnectController.hpp"
#include "Facebook.hpp"
#include "OnlineHistory.hpp"

QReadWriteLock  mutexConversation;
ConversationManager* ConversationManager::m_This = NULL;

// ===================================================================================
// Singleton


ConversationManager::ConversationManager(QObject *parent) : QObject(parent), m_OnlineHistory(NULL), m_SynchStatus(NONE), m_SynchPushLoc(0) {


    loadUserName();
}


// get singleton
ConversationManager *ConversationManager::get() {
    if(m_This == NULL) {
        // if not already done, instantiate the network manager
        m_This = new ConversationManager(NULL);
    }

    return m_This;
}


void ConversationManager::clear() {
    m_BareID = "";
    m_CurrentDst = "";

    emit cleared();
}

void ConversationManager::initOnlineHistory() {
    if(m_OnlineHistory != NULL)
        return;

    QSettings settings("Amonchakai", "Hg10");
    if(!settings.value("access_token").value<QString>().isEmpty()) {
        GoogleConnectController *google = new GoogleConnectController();
        bool check = connect(google, SIGNAL(messageLoaded(const QString &, const QString &, const QString &)), this, SLOT(onlineMessage(const QString &, const QString &, const QString &)));
        Q_ASSERT(check);
        Q_UNUSED(check);

        check = connect(google, SIGNAL(synchCompleted()), this, SLOT(saveHistory()));
        Q_ASSERT(check);

        m_OnlineHistory = google;
    } else {
        Facebook *facebook = new Facebook();
        m_OnlineHistory = facebook;
    }
}


// ===================================================================================
// User information

void ConversationManager::loadUserName() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
    if (!QFile::exists(directory)) {
        return;
    }

    QFile file(directory + "/UserID.txt");

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        stream >> m_User;
    }
    file.close();
}


// ===================================================================================
// History handling

// ---------------------------------------------------------------------------
// from a file
void ConversationManager::load(const QString &from) {
    m_CurrentDst = from;
    m_BareID = from;

    int id = m_CurrentDst.indexOf("/");
    if(id != -1)
        m_CurrentDst = m_CurrentDst.mid(0,id);

    m_History.m_History.clear();

    qDebug() << "Read history from: " << m_CurrentDst;


    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    mutexConversation.lockForRead();
    QFile file(directory + "/" + m_CurrentDst);

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        stream >> m_History;

        file.close();

    } else {
        //qDebug() << "No history";
    }

    qDebug() << "history length: " << m_History.m_History.size();

    mutexConversation.unlock();

    // ----------------------------------------------------------------
    // check if we need to load google history
    if(m_OnlineHistory == NULL) {
        initOnlineHistory();
    }

    if(m_OnlineHistory != NULL) {
        qDebug() << "m_OnlineHistory->getMessages(from, 1);";
        m_OnlineHistory->getMessages(from, 1);
        m_SynchStatus = NONE;
    }

    emit historyLoaded();

}


TimeEvent ConversationManager::getPreview() const {
    return getPreview(m_CurrentDst);
}

TimeEvent ConversationManager::getPreview(const QString &from) const {
    TimeEvent e;

    // --------------------------------------------------------------------------------
    // read preview file, and return the event!


    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    if (QFile::exists(directory)) {
        QString fromC = from;
        int id = fromC.indexOf("/");
        if(id != -1)
            fromC = fromC.mid(0,id);

        QFile file2(directory + "/" + fromC + ".preview");
        if(file2.exists()) {

            if (file2.open(QIODevice::ReadOnly)) {
            QDataStream stream(&file2);
                stream >> e;

                file2.close();
            } else {
                if(e.m_When == 0) {
                     e.m_Read = 1;
                 }
            }

        } else {
            e.m_When = 0;
            e.m_Read = 1;

        }
    }


    return e;
}

void ConversationManager::saveHistory() {

    mutexConversation.lockForWrite();

    // if nothing to save, quit.
    if(m_History.m_History.size() == 0) {
        mutexConversation.unlock();
        return;
    }


    QString fromC = m_CurrentDst;
    int id = fromC.indexOf("/");
    if(id != -1)
        fromC = fromC.mid(0,id);

    // --------------------------------------------------------------------------------
    // history file

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/" + fromC);

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        stream << m_History;

        file.close();
    }

    // --------------------------------------------------------------------------------
    // preview file

    QFile file2(directory + "/" + fromC + ".preview");

    if (file2.open(QIODevice::WriteOnly)) {
    QDataStream stream(&file2);
        stream << m_History.m_History.last();

        file2.close();
    }

    mutexConversation.unlock();
}


// ---------------------------------------------------------------------------
// from Google...
void ConversationManager::onlineMessage(const QString &from, const QString &message, const QString &messageId) {

    if(m_SynchStatus == NONE) {
        mutexConversation.lockForWrite();
        if(m_History.m_History.size() > 0) {
            if(m_History.m_History.last().m_What == message) {
                mutexConversation.unlock();
                return;
            }

            QString lastSynch;
            for(int i = m_History.m_History.size()-1 ; i >= 0 ; --i) {
                if(!m_History.m_History.at(i).m_MessageID.isEmpty()) {
                    lastSynch = m_History.m_History.at(i).m_MessageID;
                    break;
                }
            }

            m_SynchStatus = PUSH;
            qDebug() << "complete history!";

            // cleanup history...
            // delete local messages and replace them by data from Google.
            while(m_History.m_History.size() > 0 && m_History.m_History.last().m_Read < 2)
                m_History.m_History.pop_back();

            m_SynchPushLoc = m_History.m_History.size();

            // get messages up to the last full synch
            m_OnlineHistory->getRemainingMessages(lastSynch);

        } else {
            // history was empty, just push data from Google.
            m_SynchStatus = FLUSH;
            m_SynchPushLoc = 0;
            qDebug() << "Start from no history!";

            m_OnlineHistory->getRemainingMessages("");
        }

        mutexConversation.unlock();
    }



    mutexConversation.lockForWrite();

        TimeEvent e;
        e.m_Read = 1;
        e.m_What = message;
        e.m_Who = from;
        e.m_When = QDateTime::currentDateTime().toMSecsSinceEpoch();
        e.m_MessageID = messageId;
        m_History.m_History.insert(m_SynchPushLoc, e);

    mutexConversation.unlock();

    qDebug() << "push message from hist!";
    emit historyMessage(from, message);
}


void ConversationManager::deleteHistory(const QString &with) {
    QString fromC = with;
    int id = fromC.indexOf("/");
    if(id != -1)
       fromC = fromC.mid(0,id);


    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    if (QFile::exists(directory + "/" + fromC)) {
        QDir dir(directory);
        dir.remove(fromC);
    }

    if (QFile::exists(directory + "/" + fromC + ".preview")) {
        QDir dir(directory);
        dir.remove(fromC + ".preview");
    }

}


// ===================================================================================
// XMPP events

// receive message
void ConversationManager::receiveMessage(const QString &from, const QString &to, const QString &message) {

    if(message.isEmpty())
        return;

    mutexConversation.lockForWrite();

    QString fromC = from;
    int id = fromC.indexOf("/");
    if(id != -1)
        fromC = fromC.mid(0,id);

    // message sent from another computer...
    if(from.toLower() == m_User.toLower()) {
        logSent(to, message);
        return;
    }

    if(fromC.toLower() == m_CurrentDst.toLower())
        emit messageReceived(fromC, message);

    // --------------------------------------------------------------------------------
    // history file

    TimeEvent e;
    e.m_Who = fromC;
    e.m_What = message;
    e.m_Read = fromC == m_CurrentDst;
    e.m_When = QDateTime::currentDateTime().toMSecsSinceEpoch();

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/" + fromC);

    if (file.open(QIODevice::Append)) {
        QDataStream stream(&file);
        stream << e;

        file.close();
    } else {
        qDebug() << "Cannot write history";
    }

    // --------------------------------------------------------------------------------
    // preview file


    QFile file2(directory + "/" + fromC + ".preview");

    if (file2.open(QIODevice::WriteOnly)) {
    QDataStream stream(&file2);
        stream << e;

        file2.close();
    } else {
        //qDebug() << "Cannot write preview";
    }


    mutexConversation.unlock();

    if(fromC.toLower() != m_CurrentDst.toLower())
        emit messageReceived(fromC, message);

}


void ConversationManager::sendMessage(const QString &message) {
    sendMessage(m_BareID, message);
}


void ConversationManager::sendMessage(const QString &to, const QString &message) {

    if(to.isEmpty())
        return;

    XMPP::get()->sendMessageTo(to, message);

    logSent(to, message);

}

void ConversationManager::logSent(const QString &to, const QString &message) {

    mutexConversation.lockForWrite();

    if(m_User.isEmpty())
        loadUserName();

    // --------------------------------------------------------------------------------
    // history file

    TimeEvent e;
    e.m_Who = m_User;
    e.m_What = message;
    e.m_Read = 0;
    e.m_When = QDateTime::currentDateTime().toMSecsSinceEpoch();

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/" + to);

    if (file.open(QIODevice::Append)) {
        QDataStream stream(&file);
        stream << e;

        file.close();
    }


    // --------------------------------------------------------------------------------
    // preview file


    QFile file2(directory + "/" + to + ".preview");

    if (file2.open(QIODevice::WriteOnly)) {
    QDataStream stream(&file2);
        stream << e;

        file2.close();
    } else {
        //qDebug() << "Cannot write preview";
    }

    emit messageSent(to, message);

    mutexConversation.unlock();
}



// ---------------------------------------------------------------------------
// File transfer through XMPP


void ConversationManager::sendData(const QString &file) {
    XMPP::get()->sendData(file, m_BareID);
}





// ---------------------------------------------------------------------------
// Typing notification from XMPP



void ConversationManager::updateState(const QString &who, int state) {
    if(who.toLower() == m_BareID.toLower())
        emit chatStateNotify(state);
}



// ===================================================================================
// User events




void ConversationManager::markRead() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    TimeEvent e;

    QString fromC = m_CurrentDst;
    int id = fromC.indexOf("/");
    if(id != -1)
        fromC = fromC.mid(0,id);

    if (QFile::exists(directory)) {

        QFile file2(directory + "/" + fromC + ".preview");

        if (file2.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file2);
            stream >> e;

            file2.close();
        } else {
            //qDebug() << "Cannot open preview";
            return;
        }
    }

    if(e.m_What.isEmpty())
        return;

    e.m_Read = true;


    QFile file(directory + "/" + fromC + ".preview");
    if (file.open(QIODevice::WriteOnly)) {
    QDataStream stream(&file);
       stream << e;

        file.close();
    } else {
        //qDebug() << "Cannot write preview";
    }

}
