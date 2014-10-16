/*
 * AppSettings.cpp
 *
 *  Created on: 16 oct. 2014
 *      Author: pierre
 */



#include "SettingsController.hpp"
#include "ConversationMAnager.hpp"


SettingsController::SettingsController(QObject *parent) : QObject(parent) {

    m_User = ConversationManager::get()->getUser();
    m_Avatar = ConversationManager::get()->getAvatar();

    bool check = connect(ConversationManager::get(), SIGNAL(avatarUpdated()), this, SLOT(updateAvatar()));

    Q_ASSERT(check);
    Q_UNUSED(check);
}


void SettingsController::updateAvatar() {
    setAvatar(ConversationManager::get()->getAvatar());
}
