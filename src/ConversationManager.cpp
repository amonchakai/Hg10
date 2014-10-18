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

QReadWriteLock  mutexConversation;
ConversationManager* ConversationManager::m_This = NULL;


ConversationManager::ConversationManager(QObject *parent) : QObject(parent) {

    loadUserName();
}

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


// get singleton
ConversationManager *ConversationManager::get() {
    if(m_This == NULL) {
        // if not already done, instantiate the network manager
        m_This = new ConversationManager(NULL);
    }

    return m_This;
}


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
        qDebug() << "No history";
    }

    qDebug() << "history length: " << m_History.m_History.size();

    mutexConversation.unlock();

    emit historyLoaded();

}


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
    e.m_When = QDateTime::currentDateTime().toString();

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
        qDebug() << "Cannot write preview";
    }


    mutexConversation.unlock();

    if(fromC.toLower() != m_CurrentDst.toLower())
        emit messageReceived(fromC, message);

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

        if (file2.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file2);
            stream >> e;

            file2.close();
        } else {
            qDebug() << "Cannot open preview";
        }

        if(e.m_When.isEmpty()) {
            e.m_When = QDateTime(QDate(01,01,01)).toString();
            e.m_Read = 1;
        }
    }


    return e;
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
    e.m_When = QDateTime::currentDateTime().toString();

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
        qDebug() << "Cannot write preview";
    }

    emit messageSent(to, message);

    mutexConversation.unlock();
}

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
            qDebug() << "Cannot open preview";
        }
    }

    e.m_Read = true;


    QFile file(directory + "/" + fromC + ".preview");
    if (file.open(QIODevice::WriteOnly)) {
    QDataStream stream(&file);
       stream << e;

        file.close();
    } else {
        qDebug() << "Cannot write preview";
    }

}
