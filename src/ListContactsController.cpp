/*
 * ListContactsController.cpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */


#include "ListContactsController.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ArrayDataModel>
#include <bb/platform/Notification>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemDialog>

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QRegExp>
#include <QSettings>

#include "DataObjects.h"
#include "XMPPService.hpp"
#include "ConversationManager.hpp"
#include "QXmppVCardIq.h"


ListContactsController::ListContactsController(QObject *parent) : QObject(parent),
    m_ListView(NULL), m_BlackListView(NULL), m_Activity(NULL), m_OnlyFavorite(false), m_PushStated(false), m_Notification(NULL) {

    bool check = connect(XMPP::get(), SIGNAL(contactReceived()), this, SLOT(updateView()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = connect(XMPP::get(), SIGNAL(pushContact(const Contact*)), this, SLOT(pushContact(const Contact*)));
    Q_ASSERT(check);

    check = connect(ConversationManager::get(), SIGNAL(messageReceived(const QString &, const QString &)), this, SLOT(messageReceived(const QString &, const QString &)));
    Q_ASSERT(check);

    check = connect(ConversationManager::get(), SIGNAL(messageSent(const QString &, const QString &)), this, SLOT(messageReceived(const QString &, const QString &)));
    Q_ASSERT(check);

    check = connect(XMPP::get(), SIGNAL(presenceUpdated(const QString &, int)), this, SLOT(updatePresence(const QString &, int)));
    Q_ASSERT(check);

    check = connect(XMPP::get(), SIGNAL(offline(bool)), this, SLOT(updateConnectionStatus(bool)));
    Q_ASSERT(check);

    check = connect(ConversationManager::get(), SIGNAL(cleared()), this, SLOT(clear()));
    Q_ASSERT(check);

    QSettings settings("Amonchakai", "Hg10");
    m_OnlyFavorite = settings.value("FilterContacts", false).toBool();
    m_Presence  = settings.value("Presence", "").toString();
    m_Available = settings.value("Available", 0).toInt();
    m_ChoosePriority = settings.value("SetPriority", false).toBool();
    m_Priority = settings.value("Priority", 20).toInt();

    m_AvailabilityFilter = settings.value("AvailabilityFilter", -1).toInt();
    m_ConversTheme = settings.value("ConversationTheme", 0).toInt();
    m_ContactSortingKey = settings.value("ContactSortingKey", 0).toInt();

    loadBlackList();

}

void ListContactsController::clear() {
    updateView();
    setUserName("");
    setAvatar("assets://images/avatar.png");
    emit cleared();
}


void ListContactsController::deleteHistory(const QString &with) {
    using namespace bb::cascades;
    using namespace bb::system;

    ConversationManager::get()->deleteHistory(with);

    // reflect suppression on the view
    for(int i = 0 ; i < m_Contacts.size() ; ++i) {
        if(m_Contacts.at(i)->getID().toLower() == with.toLower()) {
            m_Contacts.at(i)->setPreview("");
            m_Contacts.at(i)->setTimestamp(0);
            m_Contacts.at(i)->setTimestampString("");

            break;
        }
    }
}

void ListContactsController::saveBlackList() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/blacklist.txt");

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        for(QSet<QString>::iterator it = m_BlackList.begin() ; it != m_BlackList.end() ; ++it)
            stream << *it << "\n";
        file.close();
    }

}

void ListContactsController::loadBlackList() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/");
    if (!QFile::exists(directory)) {
        return;
    }

    QFile file(directory + "/blacklist.txt");

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString id;
        m_BlackList.clear();

        stream >> id;
        while(!id.isEmpty()) {
            m_BlackList.insert(id);
            stream >> id;
        }
        file.close();
    }
}


void ListContactsController::updateConnectionStatus(bool status) {
    if(m_Activity != NULL) {
        if(status)
            m_Activity->start();
        else {
            if(!m_Contacts.isEmpty())
                m_Activity->stop();
        }
    }
}

void ListContactsController::setPresence(const QString &text, int presence, bool setPriority, int priority) {
    if(setPriority)
        XMPP::get()->setStatusText(text, presence, priority);
    else
        XMPP::get()->setStatusText(text, presence, 20);

    QSettings settings("Amonchakai", "Hg10");
    settings.setValue("Presence", text);
    settings.setValue("Available", presence);
    settings.setValue("SetPriority", setPriority);
    if(setPriority)
        settings.setValue("Priority", priority);
}

void ListContactsController::updatePresence(const QString &who, int status) {
    // lazy update to avoid refreshing completely the view.
    qDebug() << "update presence: " << who << status;
    bool wasThere = false;
    for(int i = 0 ; i < m_Contacts.size() ; ++i) {
        if(m_Contacts.at(i)->getID().toLower() == who.toLower()) {
            m_Contacts.at(i)->setPresence(status);
            wasThere = true;
            break;
        }

    }

    if(m_AvailabilityFilter != -1 && !wasThere) {
        updateView();
    }

}

void ListContactsController::addContact() {
    using namespace bb::cascades;
    using namespace bb::system;

    SystemPrompt *prompt = new SystemPrompt();
    prompt->setTitle(tr("Add a new contact"));
    prompt->setDismissAutomatically(true);
    prompt->inputField()->setEmptyText(tr("email address..."));


    bool success = QObject::connect(prompt,
        SIGNAL(finished(bb::system::SystemUiResult::Type)),
        this,
        SLOT(onPromptFinishedAddContact(bb::system::SystemUiResult::Type)));

    if (success) {
        prompt->show();
     } else {
        prompt->deleteLater();
    }
    //XMPP::get()->addContact(email);
}


void ListContactsController::onPromptFinishedAddContact(bb::system::SystemUiResult::Type result) {
    using namespace bb::cascades;
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        qDebug()<< "onPromptFinishedAddContact: " << result;

        SystemPrompt* prompt = qobject_cast<SystemPrompt*>(sender());
        if(prompt != NULL) {
            //qDebug() << "Prompt Accepted:" << prompt->inputFieldTextEntry();
            XMPP::get()->addContact(prompt->inputFieldTextEntry());

            prompt->deleteLater();
        }
    }
}

void ListContactsController::refresh() {
    XMPP::get()->getContactList();
}


void ListContactsController::messageReceived(const QString &from, const QString &message) {

    for(int i = 0 ; i < m_Contacts.size() ; ++i) {
        if(m_Contacts.at(i)->getID().toLower() == from.toLower()) {
            m_Contacts.at(i)->setPreview(message);

            QDateTime now = QDateTime::currentDateTime();
            m_Contacts.at(i)->setTimestamp(now.toMSecsSinceEpoch());
            m_Contacts.at(i)->setTimestampString(formatTime(now.toMSecsSinceEpoch()));

            bool read = ConversationManager::get()->isAdressee(from);
            m_Contacts.at(i)->setRead(read);

            if(read == 0) {
                if(m_Notification == NULL)
                    m_Notification = new bb::platform::Notification();

                m_Notification->notify();

            }

            break;
        }
    }

}

void ListContactsController::markRead() {
    ConversationManager::get()->markRead();

    if(m_Notification == NULL) {
        m_Notification = new bb::platform::Notification();
    }

    m_Notification->clearEffectsForAll();

    qDebug() << "Mark READ";


    for(int i = 0 ; i < m_Contacts.size() ; ++i) {
        if(ConversationManager::get()->isAdressee(m_Contacts.at(i)->getID())) {
            m_Contacts.at(i)->setRead(1);

            TimeEvent e = ConversationManager::get()->getPreview(m_Contacts.at(i)->getID());
            e.m_What.replace("&#39;","\'");
            e.m_What.replace("&amp;","&");
            e.m_What.replace("&euro;","€");
            e.m_What.replace("&lt;","<");
            e.m_What.replace("&gt;",">");
            e.m_What.replace(QChar(0x1F61C), ":P");

            m_Contacts.at(i)->setPreview(e.m_What);
            m_Contacts.at(i)->setTimestamp(e.m_When);
            m_Contacts.at(i)->setTimestampString(formatTime(e.m_When));

            break;
        }

    }


    ConversationManager::get()->closeConversation();
    XMPP::get()->updateHub();

}

void ListContactsController::markAllRead() {

    if(m_Notification == NULL) {
        m_Notification = new bb::platform::Notification();
    }

    m_Notification->clearEffectsForAll();

    qDebug() << "Mark all READ";


    for(int i = 0 ; i < m_Contacts.size() ; ++i) {
        m_Contacts.at(i)->setRead(1);

        TimeEvent e = ConversationManager::get()->getPreview(m_Contacts.at(i)->getID());
        e.m_What.replace("&#39;","\'");
        e.m_What.replace("&amp;","&");
        e.m_What.replace("&euro;","€");
        e.m_What.replace("&lt;","<");
        e.m_What.replace("&gt;",">");
        e.m_What.replace(QChar(0x1F61C), ":P");

        m_Contacts.at(i)->setPreview(e.m_What);
        m_Contacts.at(i)->setTimestamp(e.m_When);
        m_Contacts.at(i)->setTimestampString(formatTime(e.m_When));

        ConversationManager::get()->markRead(m_Contacts.at(i)->getID());

    }


    ConversationManager::get()->closeConversation();

}

void ListContactsController::selectFirst() {
    if(m_Contacts.isEmpty())
        return;

    emit userSelected(m_Contacts.first()->getID(), m_Contacts.first()->getName(), m_Contacts.first()->getAvatar());
}


void ListContactsController::updateView() {

    m_PushStated = false;

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if (dataModel) {
        dataModel->clear();
    } else {
        qDebug() << "create new model";
        dataModel = new GroupDataModel(
                QStringList() << "name"
                              << "id"
                              << "timestamp"
                              << "timestampString"
                              << "avatar"
                              << "preview"
                              << "presence"
                              << "read"
                );
        m_ListView->setDataModel(dataModel);
    }
    //dataModel->setGrouping(ItemGrouping::ByFullValue);

    QStringList keys;
    switch(m_ContactSortingKey) {
        case 0: {
            keys.push_back("timestamp");
            keys.push_back("name");
            dataModel->setGrouping(ItemGrouping::None);
            dataModel->setSortingKeys(keys);
            dataModel->setSortedAscending(false);
            break;
        }

        case 1: {
            keys.push_back("name");
            dataModel->setGrouping(ItemGrouping::ByFirstChar);
            dataModel->setSortingKeys(keys);
            dataModel->setSortedAscending(true);
            break;
        }
    }

    // ----------------------------------------------------------------------------------------------
    // Read login info

    m_Contacts.clear();

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    QDateTime now = QDateTime::currentDateTime();

    const QList<Contact *> *contacts = XMPP::get()->getContacts();

    QList<QObject*> datas;
    for(int i = contacts->length()-1 ; i >= 0 ; --i) {
        if(m_BlackList.find(contacts->at(i)->getID()) != m_BlackList.end()) continue;

        // remove yourself from the list of contact, and store the info for display
        if(contacts->at(i)->getID().toLower() != ConversationManager::get()->getUser().toLower() && !contacts->at(i)->getID().isEmpty()) {

            TimeEvent e = ConversationManager::get()->getPreview(contacts->at(i)->getID());
            if(m_OnlyFavorite && e.m_What.isEmpty())
                 continue;

            if(m_AvailabilityFilter == 1 && contacts->at(i)->getPresence() != 0)
                continue;

            if((m_AvailabilityFilter/10) == 1 && contacts->at(i)->getPresence() == -1)
                continue;

            bool skip = false;
            for(int k = 0 ; k < m_Contacts.size() ; ++k) {
                if(contacts->at(i)->getID() == m_Contacts.at(k)->getID()) {
                    if(QFile::exists(contacts->at(i)->getAvatar()) || contacts->at(i)->getAvatar() == "asset:///images/avatar.png") {
                        m_Contacts.at(k)->setAvatar(contacts->at(i)->getAvatar());
                        skip = true;
                        break;
                    }
                }
            }
            if(skip)
                continue;


            Contact *nc = new Contact;
            if(QFile::exists(contacts->at(i)->getAvatar()))
                nc->setAvatar(contacts->at(i)->getAvatar());
            else
                nc->setAvatar("asset:///images/avatar.png");
            nc->setName(contacts->at(i)->getName());

            nc->setTimestamp(e.m_When);
            nc->setTimestampString(formatTime(e.m_When));

            e.m_What.replace("&#39;","\'");
            e.m_What.replace("&amp;","&");
            e.m_What.replace("&euro;","€");
            e.m_What.replace("&lt;","<");
            e.m_What.replace("&gt;",">");
            e.m_What.replace(QChar(0x1F61C), ":P");

            nc->setPreview(e.m_What);

            nc->setID(contacts->at(i)->getID());
            nc->setPresence(contacts->at(i)->getPresence());
            nc->setRead(e.m_Read);

            datas.push_back(nc);
            m_Contacts.push_back(nc);

            e.m_When = 0;

        } else {
            if(contacts->at(i)->getID().isEmpty())
                setUserName(contacts->at(i)->getName());
            else {
                if(m_User.isEmpty())
                    setUserName(contacts->at(i)->getName());
            }
        }
    }

    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");
    QString avatar(vCardsDir + "/" + ConversationManager::get()->getUser() + ".png");
    if(QFile::exists(avatar)) {
        setAvatar(avatar);
        if(ConversationManager::get()->getAvatar() != avatar)
            ConversationManager::get()->setAvatar(avatar);
    } else {
        setAvatar("asset:///images/avatar.png");
        if(ConversationManager::get()->getAvatar() != "asset:///images/avatar.png")
            ConversationManager::get()->setAvatar("asset:///images/avatar.png");
    }



    dataModel->clear();
    dataModel->insertList(datas);

    if(m_Activity != NULL && !contacts->isEmpty()) {
        m_Activity->stop();
    }
}

void ListContactsController::pushContact(const Contact* c) {
    if(m_BlackList.find(c->getID()) != m_BlackList.end()) {
        return;
    }

    if(m_Activity != NULL)
        m_Activity->stop();

    qDebug() << "contact : " << c->getID();

    using namespace bb::cascades;

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());

    // check if not already in the list...
    for(int i = 0 ; i < m_Contacts.length() ; ++i) {
        if(m_Contacts.at(i)->getID() == c->getID())
            return;
    }

    if(c->getID().toLower() != ConversationManager::get()->getUser().toLower()  && !c->getID().isEmpty()) {
        TimeEvent e = ConversationManager::get()->getPreview(c->getID());

        if(m_OnlyFavorite && e.m_What.isEmpty())
            return;

        if(m_AvailabilityFilter == 1 && c->getPresence() != 0)
            return;

        if((m_AvailabilityFilter/10) == 1 && c->getPresence() == -1)
            return;

        Contact *nc = new Contact;
        nc->setAvatar(c->getAvatar());
        nc->setName(c->getName());

        nc->setTimestamp(e.m_When);
        nc->setTimestampString(formatTime(e.m_When));

        e.m_What.replace("&#39;","\'");
        e.m_What.replace("&amp;","&");
        e.m_What.replace("&euro;","€");
        e.m_What.replace("&lt;","<");
        e.m_What.replace("&gt;",">");
        e.m_What.replace(QChar(0x1F61C), ":P");

        nc->setPreview(e.m_What);

        nc->setID(c->getID());
        nc->setPresence(c->getPresence());
        nc->setRead(e.m_Read);

        m_Contacts.push_back(nc);

        e.m_When = 0;

        dataModel->insert(nc);

    } else {
        if(c->getID().isEmpty())
            setUserName(c->getName());
        else {
            if(m_User.isEmpty())
                setUserName(c->getName());
        }
    }

    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");
    QString avatar(vCardsDir + "/" + ConversationManager::get()->getUser() + ".png");
    if(QFile::exists(avatar)) {
        setAvatar(avatar);
        if(ConversationManager::get()->getAvatar() != avatar)
            ConversationManager::get()->setAvatar(avatar);
    } else {
        setAvatar("asset:///images/avatar.png");
        if(ConversationManager::get()->getAvatar() != "asset:///images/avatar.png")
            ConversationManager::get()->setAvatar("asset:///images/avatar.png");
    }

}

void ListContactsController::filter(const QString &contact) {

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if (!dataModel) {
        qDebug() << "create new model";
        dataModel = new GroupDataModel(
                QStringList() << "name"
                              << "id"
                              << "timestamp"
                              << "timestampString"
                              << "avatar"
                              << "preview"
                              << "presence"
                              << "read"
                );
        m_ListView->setDataModel(dataModel);
    }
    //dataModel->setGrouping(ItemGrouping::ByFullValue);

    QStringList keys;
    switch(m_ContactSortingKey) {
        case 0: {
            keys.push_back("timestamp");
            keys.push_back("name");
            dataModel->setGrouping(ItemGrouping::None);
            dataModel->setSortingKeys(keys);
            dataModel->setSortedAscending(false);
            break;
        }

        case 1: {
            keys.push_back("name");
            dataModel->setGrouping(ItemGrouping::ByFirstChar);
            dataModel->setSortingKeys(keys);
            dataModel->setSortedAscending(true);
            break;
        }
    }

    // ----------------------------------------------------------------------------------------------
    // Read login info

    m_Contacts.clear();

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    const QList<Contact *> *contacts = XMPP::get()->getContacts();

    QList<QObject *> datas;
    for(int i = contacts->length()-1 ; i >= 0 ; --i) {
        if(m_BlackList.find(contacts->at(i)->getID()) != m_BlackList.end()) continue;

        // remove yourself from the list of contact, and store the info for display
        if(contacts->at(i)->getID().toLower() != ConversationManager::get()->getUser().toLower() &&
           contacts->at(i)->getName().mid(0, contact.length()).toLower() == contact.toLower()) {

            bool skip = false;
            for(int k = 0 ; k < m_Contacts.size() ; ++k) {
                if(contacts->at(i)->getID() == m_Contacts.at(k)->getID()) {
                    if(QFile::exists(contacts->at(i)->getAvatar()) || contacts->at(i)->getAvatar() == "asset:///images/avatar.png") {
                        m_Contacts.at(k)->setAvatar(contacts->at(i)->getAvatar());
                        skip = true;
                        break;
                    }
                }
            }
            if(skip)
                continue;


            TimeEvent e = ConversationManager::get()->getPreview(contacts->at(i)->getID());

            Contact *nc = new Contact;
            if(QFile::exists(contacts->at(i)->getAvatar()))
                nc->setAvatar(contacts->at(i)->getAvatar());
            else
                nc->setAvatar("asset:///images/avatar.png");
            nc->setName(contacts->at(i)->getName());

            nc->setTimestamp(e.m_When);
            nc->setTimestampString(formatTime(e.m_When));

            e.m_What.replace("&#39;","\'");
            e.m_What.replace("&amp;","&");
            e.m_What.replace("&euro;","€");
            e.m_What.replace("&lt;","<");
            e.m_What.replace("&gt;",">");
            e.m_What.replace(QChar(0x1F61C), ":P");

            nc->setPreview(e.m_What);

            nc->setID(contacts->at(i)->getID());
            nc->setPresence(contacts->at(i)->getPresence());
            nc->setRead(e.m_Read);

            m_Contacts.push_back(nc);
            datas.push_back(nc);

            e.m_When = 0;

        }
    }

    dataModel->clear();
    dataModel->insertList(datas);

}

void ListContactsController::setFilter(bool onlyFav)  {
    if(m_OnlyFavorite == onlyFav)
        return;

    m_OnlyFavorite = onlyFav;

    QSettings settings("Amonchakai", "Hg10");
    settings.setValue("FilterContacts", m_OnlyFavorite);

    updateView();
    emit showOnlyFavChanged();
}

void ListContactsController::setAvailabilityFilter(int level) {
    if(m_AvailabilityFilter == level)
        return;

    qDebug() << "Set filter: " << level;

    m_AvailabilityFilter = level;

    QSettings settings("Amonchakai", "Hg10");
    settings.setValue("AvailabilityFilter", m_AvailabilityFilter);

    updateView();
    emit availabilityFilterChanged();
}

void ListContactsController::setContactSortingKey(int index) {
    if(m_ContactSortingKey == index)
        return;

    m_ContactSortingKey = index;

    QSettings settings("Amonchakai", "Hg10");
    settings.setValue("ContactSortingKey", m_ContactSortingKey);

    updateView();
    emit contactSortingKeyChanged();
}

QString ListContactsController::formatTime(qint64 msecs) {
    if(msecs == 0)
        return "";

    QDateTime now = QDateTime::currentDateTime();
    QDateTime time = QDateTime::fromMSecsSinceEpoch(msecs);

    if(now.date() == time.date()) {
        return time.time().toString("hh:mm");
    } else {

        return time.date().toString("dd.MM.yyyy");
    }
}


void ListContactsController::loadContactDetails(const QString &id) {
    // -------------------------------------------------------------
    // edit the vcard...

    qDebug() << "load contact details";

    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");

    QDomDocument doc("vCard");

    {
        QFile file(vCardsDir + "/" + id + ".xml");
        if (!file.open(QIODevice::ReadOnly))
            return;
        if (!doc.setContent(&file)) {
            //file.close();
            //return;
            qWarning() << "Problem while reading vcard: " << id << " file not complete, some data may be missing.";

        }
        file.close();
    }

    QXmppVCardIq vCard;
    vCard.parse(doc.documentElement());

    setFirstName(vCard.firstName());
    setLastName(vCard.lastName());
    setNickname(vCard.nickName());
}

void ListContactsController::removeFromBlackList(const QString &id) {
    m_BlackList.remove(id);
    saveBlackList();
    updateBlackList();
}

void ListContactsController::blacklistContact(const QString &id) {
    using namespace bb::cascades;
    using namespace bb::system;

    m_tmp_blacklist = id;

    SystemDialog *dialog = new SystemDialog("Yes", "No");

    dialog->setTitle(tr("Blacklist"));
    dialog->setBody(tr("Are you sure to blacklist this contact?"));

    bool success = connect(dialog,
         SIGNAL(finished(bb::system::SystemUiResult::Type)),
         this,
         SLOT(onPromptFinishedBlacklist(bb::system::SystemUiResult::Type)));

    if (success) {
        dialog->show();
    } else {
        dialog->deleteLater();
    }
}

void ListContactsController::onPromptFinishedBlacklist(bb::system::SystemUiResult::Type result) {
    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        m_BlackList.insert(m_tmp_blacklist);
        saveBlackList();
        updateView();
    }
}

void ListContactsController::updateBlackList() {
    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;


    if(m_BlackListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    ArrayDataModel* dataModel = dynamic_cast<ArrayDataModel*>(m_BlackListView->dataModel());
    if (dataModel) {
        dataModel->clear();
    }

    // ----------------------------------------------------------------------------------------------
    // push data to the view
    for(QSet<QString>::iterator it = m_BlackList.begin() ; it != m_BlackList.end() ; ++it) {
        dataModel->append(*it);
    }
}


void ListContactsController::editContact(const QString &id, const QString &fullname, const QString &firstname, const QString &lastname, const QString &nickname) {
    for(int i = 0 ; i < m_Contacts.size() ; ++i) {
        if(m_Contacts.at(i)->getID().toLower() == id.toLower()) {
            m_Contacts.at(i)->setName(fullname);
            break;
        }
    }






    // -------------------------------------------------------------
    // edit the vcard...

    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");

    QDomDocument doc("vCard");

    {
        QFile file(vCardsDir + "/" + id + ".xml");
        if (!file.open(QIODevice::ReadOnly))
            return;
        if (!doc.setContent(&file)) {
            //file.close();
            //return;
            qWarning() << "Problem while reading vcard: " << id << " file not complete, some data may be missing.";

        }
        file.close();
    }

    QXmppVCardIq vCard;
    vCard.parse(doc.documentElement());
    vCard.setFullName(fullname);
    vCard.setFirstName(firstname);
    vCard.setLastName(lastname);
    vCard.setNickName(nickname);

    {
        QFile file(vCardsDir + "/" + id + ".xml");

        if(file.open(QIODevice::ReadWrite)) {
            QXmlStreamWriter stream(&file);
            vCard.toXml(&stream);
            file.close();
        }
    }

}

