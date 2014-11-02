/*
 * XMPPController.cpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */


#include "LoginController.hpp"
#include "XMPPService.hpp"
#include "ConversationManager.hpp"
#include <QFile>
#include <QDir>
#include <bb/system/SystemToast>

#include <QSettings>

LoginController::LoginController(QObject *parent) : QObject(parent) {
    if(isLogged()) {
        loadUserName();
        XMPP::get()->connectToServer(m_User, m_Password);
    }

    {
        QSettings setting("Amonchakai", "Hg10");
        if(setting.contains("notifications"))
            m_Notif = setting.value("notification").value<bool>();
    }
}

void LoginController::setNotif(bool value) {
    m_Notif = value;

    QSettings setting("Amonchakai", "Hg10");
    setting.setValue("notification", m_Notif);

    XMPP::get()->notifySettingChange();

    emit notifChanged();
}


void LoginController::login(const QString& login, const QString &password) {
    m_User = login;
    m_Password = password;

    m_User.replace(" ", ""); // remove eventual padding spaces...
    XMPP::get()->connectToServer(m_User, password);

    bool check = QObject::connect(XMPP::get(), SIGNAL(connectedXMPP()), this, SLOT(connected()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = QObject::connect(XMPP::get(), SIGNAL(connectionFailed()), this, SLOT(connectionFailed()));
    Q_ASSERT(check);
}


void LoginController::connected() {
    QObject::disconnect(XMPP::get(), SIGNAL(connectedXMPP()), this, SLOT(connected()));
    saveUserName();
    ConversationManager::get()->loadUserName();
    emit complete();
}

void LoginController::connectionFailed() {
    QObject::disconnect(XMPP::get(), SIGNAL(connectionFailed()), this, SLOT(connectionFailed()));

    bb::system::SystemToast *toast = new bb::system::SystemToast(this);

    toast->setBody(tr("Login failed, please check if your user name and password are correct"));
    toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
    toast->show();

    emit connectError();
}



void LoginController::saveUserName() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/UserID.txt");

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        stream << m_User;
        stream << m_Password;

        file.close();
    }
}

void LoginController::loadUserName() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
    if (!QFile::exists(directory)) {
        return;
    }

    QFile file(directory + "/UserID.txt");

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        stream >> m_User;
        stream >> m_Password;

        file.close();
    }
}

void LoginController::logOut() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
    if (!QFile::exists(directory)) {
        return;
    }

    {
        QFile file(directory + "/UserID.txt");
        file.remove();
    }

    XMPP::get()->disconnectFromServer();
    XMPP::get()->clear();


}

void LoginController::deleteHistory() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/History");
    if (QFile::exists(directory)) {
        QDir dir(directory);
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList()) {
            dir.remove(dirFile);
        }
    }
}

void LoginController::clearContactsData() {
    QString directory = QDir::homePath() + QLatin1String("/vCards");
    if (QFile::exists(directory)) {
        QDir dir(directory);
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList()) {
            dir.remove(dirFile);
        }
    }
}


bool LoginController::isLogged() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
    return QFile::exists(directory + "/UserID.txt");
}


