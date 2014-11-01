/*
 * ListContactsController.cpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */


#include "ListContactsController.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/platform/Notification>
#include <QDebug>
#include <QRegExp>

#include "DataObjects.h"
#include "XMPPService.hpp"
#include "ConversationManager.hpp"

#include <QSettings>

ListContactsController::ListContactsController(QObject *parent) : QObject(parent),
    m_ListView(NULL), m_Activity(NULL), m_OnlyFavorite(false), m_PushStated(false), m_Notif(true), m_Notification(NULL) {

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

    {
        QSettings setting("Amonchakai", "Hg10");
        if(setting.contains("notifications"))
            m_Notif = setting.value("notification").value<bool>();
    }
}

void ListContactsController::clear() {
    updateView();
    setUserName("");
    setAvatar("assets://images/avatar.png");
    emit cleared();
}

void ListContactsController::setNotif(bool value) {
    m_Notif = value;

    QSettings setting("Amonchakai", "Hg10");
    setting.setValue("notification", m_Notif);

    XMPP::get()->notifySettingChange();

    emit notifChanged();
}

void ListContactsController::deleteHistory(const QString &with) {
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

void ListContactsController::updateConnectionStatus(bool status) {
    if(m_Activity != NULL) {
        if(status)
            m_Activity->start();
        else
            m_Activity->stop();
    }
}

void ListContactsController::updatePresence(const QString &who, int status) {
    // lazy update to avoid refreshing completely the view.

    for(int i = 0 ; i < m_Contacts.size() ; ++i) {
        if(m_Contacts.at(i)->getID().toLower() == who.toLower()) {

            m_Contacts.at(i)->setPresence(status);

            break;
        }
    }

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
                if(m_Notif) {
                    if(m_Notification == NULL)
                        m_Notification = new bb::platform::Notification();

                    m_Notification->notify();
                }

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

            m_Contacts.at(i)->setPreview(e.m_What);
            m_Contacts.at(i)->setTimestamp(e.m_When);
            m_Contacts.at(i)->setTimestampString(formatTime(e.m_When));

            break;
        }

    }


    ConversationManager::get()->closeConversation();

}

void ListContactsController::updateView() {

    m_PushStated = false;

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    qDebug() << "UPDATE VIEW";

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

    // ----------------------------------------------------------------------------------------------
    // Read login info

    m_Contacts.clear();

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    QDateTime now = QDateTime::currentDateTime();

    const QList<Contact *> *contacts = XMPP::get()->getContacts();

    QList<QObject*> datas;
    for(int i = contacts->length()-1 ; i >= 0 ; --i) {
        // remove yourself from the list of contact, and store the info for display
        if(contacts->at(i)->getID().toLower() != ConversationManager::get()->getUser().toLower()) {

            TimeEvent e = ConversationManager::get()->getPreview(contacts->at(i)->getID());

            Contact *nc = new Contact;
            nc->setAvatar(contacts->at(i)->getAvatar());
            nc->setName(contacts->at(i)->getName());

            nc->setTimestamp(e.m_When);
            nc->setTimestampString(formatTime(e.m_When));

            e.m_What.replace("&#39;","\'");
            e.m_What.replace(QChar(0x1F61C), ":P");

            nc->setPreview(e.m_What);

            nc->setID(contacts->at(i)->getID());
            nc->setPresence(contacts->at(i)->getPresence());
            nc->setRead(e.m_Read);

            datas.push_back(nc);
            m_Contacts.push_back(nc);

            e.m_When = 0;

        } else {
            setUserName(contacts->at(i)->getName());
            setAvatar(contacts->at(i)->getAvatar());

            if(ConversationManager::get()->getAvatar().isEmpty())
                ConversationManager::get()->setAvatar(contacts->at(i)->getAvatar());
        }
    }

    dataModel->clear();
    dataModel->insertList(datas);

    if(m_Activity != NULL)
        m_Activity->stop();
}

void ListContactsController::pushContact(const Contact* c) {

    using namespace bb::cascades;

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());

    if(!m_PushStated) {
        dataModel->clear();
        m_Contacts.clear();
        m_PushStated = true;

        qDebug() << "PUSH CONTACT!!";
    }

    // check if not already in the list...
    for(int i = 0 ; i < m_Contacts.size() ; ++i) {
        if(c->getID() == m_Contacts.at(i)->getID())
            return;
    }

    qDebug() << "Pushing: " << c->getName();

    QDateTime now = QDateTime::currentDateTime();

    if(c->getID().toLower() != ConversationManager::get()->getUser().toLower()) {

        TimeEvent e = ConversationManager::get()->getPreview(c->getID());

        Contact *nc = new Contact;
        nc->setAvatar(c->getAvatar());
        nc->setName(c->getName());

        nc->setTimestamp(e.m_When);
        nc->setTimestampString(formatTime(e.m_When));

        e.m_What.replace("&#39;","\'");
        e.m_What.replace(QChar(0x1F61C), ":P");

        nc->setPreview(e.m_What);

        nc->setID(c->getID());
        nc->setPresence(c->getPresence());
        nc->setRead(e.m_Read);

        m_Contacts.push_back(nc);

        e.m_When = 0;

        dataModel->insert(nc);

    } else {
        setUserName(c->getName());
        setAvatar(c->getAvatar());

        if(ConversationManager::get()->getAvatar().isEmpty())
            ConversationManager::get()->setAvatar(c->getAvatar());
    }

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
