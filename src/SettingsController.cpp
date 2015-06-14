/*
 * AppSettings.cpp
 *
 *  Created on: 16 oct. 2014
 *      Author: pierre
 */


#include "SettingsController.hpp"
#include "ConversationMAnager.hpp"
#include <QRegExp>
#include "XMPPService.hpp"
#include <bb/platform/Notification>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemToast>
#include "PrivateAPIKeys.h"

bool SettingsController::m_LogEnabled = false;


SettingsController::SettingsController(QObject *parent) : QObject(parent), m_FontSize(28), m_Settings(NULL) {

    m_User = ConversationManager::get()->getUser();
    m_Avatar = ConversationManager::get()->getAvatar();
    m_Settings = new QSettings("Amonchakai", "Hg10");

    m_Theme = m_Settings->value("theme").value<int>();
    m_FontSize = m_Settings->value("fontSize").value<int>();
    m_ConversTheme = m_Settings->value("ConversationTheme", 0).value<int>();
    m_Space = m_Settings->value("Space", 0).value<int>();

    if(m_FontSize == 0)
        m_FontSize = 28;
    m_LogEnabled = m_Settings->value("logsEnabled", false).toBool();

    QRegExp isFacebook("(.*)@chat.facebook.com");
    if(isFacebook.indexIn(m_User) != -1)
        m_IsGoogleEnabled = false;
    else
        m_IsGoogleEnabled = true;

    m_DropBoxEnabled = m_Settings->value("DropBoxEnabled", false).toBool();

    bool check = connect(ConversationManager::get(), SIGNAL(avatarUpdated()), this, SLOT(updateAvatar()));

    Q_ASSERT(check);
    Q_UNUSED(check);

}

bool SettingsController::getGoogleLogged() {
    return !m_Settings->value("access_token").toString().isEmpty();
}


void SettingsController::updateAvatar() {
    setAvatar(ConversationManager::get()->getAvatar());
    setUserName(ConversationManager::get()->getUser());

    QRegExp isFacebook("(.*)@chat.facebook.com");
    if(isFacebook.indexIn(ConversationManager::get()->getUser()) != -1)
        setEnableGoogle(false);

}


void SettingsController::save() {
    m_Settings->setValue("theme", m_Theme);
    m_Settings->setValue("fontSize", m_FontSize);
    m_Settings->setValue("logsEnabled", m_LogEnabled);
    m_Settings->setValue("DropBoxEnabled", m_DropBoxEnabled);
    m_Settings->setValue("ConversationTheme", m_ConversTheme);
    m_Settings->setValue("Space", m_Space);

    XMPP::get()->notifySettingChange();
}

void SettingsController::requestHubRemoval() {
    using namespace bb::system;

    SystemPrompt *prompt = new SystemPrompt();
    prompt->setTitle(tr("This feature is password protected"));
    prompt->setDismissAutomatically(true);
    prompt->inputField()->setEmptyText(tr("password"));


    bool success = QObject::connect(prompt,
        SIGNAL(finished(bb::system::SystemUiResult::Type)),
        this,
        SLOT(onPromptFinishedPassword(bb::system::SystemUiResult::Type)));

    if (success) {
        prompt->show();
     } else {
        prompt->deleteLater();
    }
}

void SettingsController::onPromptFinishedPassword(bb::system::SystemUiResult::Type result) {
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        qDebug()<< "onPromptFinishedAddContact: " << result;

        SystemPrompt* prompt = qobject_cast<SystemPrompt*>(sender());
        if(prompt != NULL) {

            if(prompt->inputFieldTextEntry() == APPLICATION_PASSWORD) {

                bb::system::SystemToast *toast = new bb::system::SystemToast(this);

                toast->setBody(tr("Accounts deleted..."));
                toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
                toast->show();

                XMPP::get()->requestHubRemoval();
            } else {
                bb::system::SystemToast *toast = new bb::system::SystemToast(this);

                toast->setBody(tr("Wrong password."));
                toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
                toast->show();
            }

            prompt->deleteLater();
        }
    }
}



