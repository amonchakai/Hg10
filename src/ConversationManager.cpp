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
#include "OnlineHistory.hpp"
#include "FileTransfert.hpp"

QReadWriteLock  mutexConversation;
ConversationManager* ConversationManager::m_This = NULL;

// ===================================================================================
// Singleton


ConversationManager::ConversationManager(QObject *parent) : QObject(parent), m_OnlineHistory(NULL), m_FileTransfert(NULL), m_SynchPushLoc(0) {
    m_GStackIndex = 0;

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
        m_OnlineHistory = google;
        m_FileTransfert = google;

        bool check = connect(m_FileTransfert, SIGNAL(picasaImageFound(const QString &, const QString &)), this, SLOT(fowardImageReceived(const QString &, const QString &)));
        Q_ASSERT(check);
        Q_UNUSED(check);

    }
}


// ===================================================================================
// User information

void ConversationManager::loadUserName() {
    if(!m_User.isEmpty())
        return;

    QSettings settings("Amonchakai", "Hg10");
    setUser(settings.value("User").toString());
}

void ConversationManager::setUser(const QString &user) {
    m_User = user;

    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");
    QString avatar(vCardsDir + "/" + m_User + ".png");
    if(QFile::exists(avatar)) {
        setAvatar(avatar);
    } else {
        setAvatar("asset:///images/avatar.png");
    }

}


// ===================================================================================
// History handling

// ---------------------------------------------------------------------------
// from a file
void ConversationManager::load(const QString &from, const QString &name) {
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

    // make a local stack of images --> avoid to lose pictures while loading the history from gmail
    // the first one is the stickers, the second one are images hosted in picasa aka G+
    m_GImageStack.clear();
    for(int i = 0 ; i < m_History.m_History.size() ; ++i) {
        if(m_History.m_History.at(i).m_What.mid(0, 33) == "https://lh3.googleusercontent.com" || m_History.m_History.at(i).m_What.mid(0, 37) == "https://plus.google.com/photos/albums" ) {
            m_GImageStack.push_back(m_History.m_History.at(i).m_What);
        }
    }
    m_GStackIndex = m_GImageStack.size()-1;

    mutexConversation.unlock();

    // ----------------------------------------------------------------
    // check if we need to load google history
    if(m_OnlineHistory == NULL) {
        initOnlineHistory();
    }

    if(m_OnlineHistory != NULL) {
        QRegExp publicTalk("@public.talk.google.com");
        if(publicTalk.indexIn(from) != -1) {
            qDebug() << "m_OnlineHistory->getMessages(\" from:(" + name + ") OR to:(" + name + ") \", 12);";
            m_OnlineHistory->getMessages(" from:(" + name + ") OR to:(" + name +")", 12);  // for google, user id is better, but not always available. If not, use name.
        } else {
            qDebug() << "m_OnlineHistory->getMessages(\"" + from + "\", 12);";
            m_OnlineHistory->getMessages(from, 12);
        }
    }

    emit historyLoaded();
    m_HistoryIndex = 10;

}

void ConversationManager::loadMore() {

    if(m_HistoryIndex <= m_History.m_History.size()) {
        for(int i = 1 ; i < 10 ; ++i) {
            if((m_History.m_History.size()-m_HistoryIndex-i) >= 0) {
                m_HistoryIndex++;
                emit historyMessageNoFlush(m_History.m_History.at(m_History.m_History.size()-m_HistoryIndex).m_Who, m_History.m_History.at(m_History.m_History.size()-m_HistoryIndex).m_What);
                qDebug() << "local";
            } else {
                m_HistoryIndex++;
                return;
            }
        }
    } else {

        if(m_OnlineHistory != NULL) {
            qDebug() << "online";
            m_OnlineHistory->getMoreMessages();
        }
    }
}


void ConversationManager::reload() {
    m_OnlineHistory->getMessages(m_CurrentDst);
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

void ConversationManager::flushHistory() {
    mutexConversation.lockForWrite();
    m_History.m_History.clear();
    mutexConversation.unlock();

    emit historyLoaded();
}

void ConversationManager::saveHistory() {

    mutexConversation.lockForWrite();

    qDebug() << "push history: " << m_CurrentDst;

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

    emit synchDone();

    m_HistoryIndex = m_History.m_History.size()+1 ;
}


// ---------------------------------------------------------------------------
// from Google...
void ConversationManager::onlineMessage(const QString &from, const QString &message, const QString &messageId) {

    mutexConversation.lockForWrite();
    if(m_History.m_History.size() > 0) {
        if(m_History.m_History.last().m_What == message) {
            mutexConversation.unlock();
            qDebug() << "History up to date!";
            return;
        }
    }


    TimeEvent e;
    e.m_Read = 1;
    e.m_What = message;
    e.m_Who = from;
    e.m_When = QDateTime::currentDateTime().toMSecsSinceEpoch();
    e.m_MessageID = messageId;

    if(e.m_What.isEmpty()) {
        if(!m_GImageStack.isEmpty() && m_GStackIndex >= 0) {
            e.m_What = m_GImageStack.at(m_GStackIndex);
            --m_GStackIndex;
        }
    }

    if(!e.m_What.isEmpty())
        m_History.m_History.insert(m_SynchPushLoc, e);

    mutexConversation.unlock();

    if(!e.m_What.isEmpty())
        emit historyMessage(from, e.m_What);
}


void ConversationManager::deleteHistory(const QString &with) {

    QString fromC;
    if(with.isEmpty())
        fromC = m_CurrentDst;
    else
        fromC = with;

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

void ConversationManager::getPictureFromLink(const QString& user_id, const QString &picture_id) {
    if(m_FileTransfert != NULL)
        m_FileTransfert->getPictureFromLink(user_id, picture_id);
}

void ConversationManager::fowardImageReceived(const QString& id, const QString& url) {
    emit imageURLFetched(id, url);
}


void ConversationManager::markRead() {
    markRead(m_CurrentDst);
}


void ConversationManager::markRead(const QString &who) {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    TimeEvent e;

    QString fromC = who;
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
