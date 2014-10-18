/*
 * AppSettings.cpp
 *
 *  Created on: 16 oct. 2014
 *      Author: pierre
 */



#include "SettingsController.hpp"
#include "ConversationMAnager.hpp"


SettingsController::SettingsController(QObject *parent) : QObject(parent), m_Settings(NULL) {

    m_User = ConversationManager::get()->getUser();
    m_Avatar = ConversationManager::get()->getAvatar();
    m_Settings = new QSettings("Amonchakai", "Hg10");

    m_Theme = m_Settings->value("theme").value<int>();

    bool check = connect(ConversationManager::get(), SIGNAL(avatarUpdated()), this, SLOT(updateAvatar()));

    Q_ASSERT(check);
    Q_UNUSED(check);
}


void SettingsController::updateAvatar() {
    setAvatar(ConversationManager::get()->getAvatar());
}


void SettingsController::save() {
    m_Settings->setValue("theme", m_Theme);
}
