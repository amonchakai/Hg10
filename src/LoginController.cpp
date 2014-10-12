/*
 * XMPPController.cpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */


#include "LoginController.hpp"
#include "XMPPService.hpp"
#include <QFile>
#include <QDir>

LoginController::LoginController(QObject *parent) : QObject(parent) {
    if(isLogged()) {
        loadUserName();
        XMPP::get()->connectToServer(m_User, m_Password);
    }
}


void LoginController::login(const QString& login, const QString &password) {
    XMPP::get()->connectToServer(login, password);
    m_User = login;
    m_Password = password;

    QObject::connect(XMPP::get(), SIGNAL(connected()), this, SLOT(connected()));
}


void LoginController::connected() {
    QObject::disconnect(XMPP::get(), SIGNAL(connected()), this, SLOT(connected()));
    saveUserName();
    emit complete();
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

}

bool LoginController::isLogged() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
    return QFile::exists(directory + "/UserID.txt");
}


