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

    m_History.m_History.clear();

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    mutexConversation.lockForRead();
    QFile file(directory + "/" + from);

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        stream >> m_History;

        file.close();
    }
    mutexConversation.unlock();

    emit historyLoaded();

}


void ConversationManager::receiveMessage(const QString &from, const QString &message) {
    mutexConversation.lockForWrite();

    Event e;
    e.m_Who = from;
    e.m_What = message;
    e.m_Read = 0;
    e.m_When = QDateTime::currentDateTime().toString();

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/" + from);

    if (file.open(QIODevice::Append)) {
        QDataStream stream(&file);
        stream << e;

        file.close();
    }

    mutexConversation.unlock();
}



void ConversationManager::sendMessage(const QString &to, const QString &message) {
    mutexConversation.lockForWrite();

    if(m_User.isEmpty())
        loadUserName();

    Event e;
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

    mutexConversation.unlock();
}




