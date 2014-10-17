/*
 * ListContactsController.cpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */


#include "ListContactsController.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <QDebug>
#include <QRegExp>

#include "DataObjects.h"
#include "XMPPService.hpp"
#include "ConversationManager.hpp"

ListContactsController::ListContactsController(QObject *parent) : QObject(parent),
    m_ListView(NULL) {

    bool check = connect(XMPP::get(), SIGNAL(contactReceived()), this, SLOT(updateView()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = connect(ConversationManager::get(), SIGNAL(messageReceived(const QString &, const QString &)), this, SLOT(messageReceived(const QString &, const QString &)));
    Q_ASSERT(check);

    check = connect(ConversationManager::get(), SIGNAL(messageSent(const QString &, const QString &)), this, SLOT(messageReceived(const QString &, const QString &)));
    Q_ASSERT(check);
}


void ListContactsController::updateContacts() {
    //qDebug() << presence.
}

void ListContactsController::messageReceived(const QString &from, const QString &message) {
    // updateView();

    // lazy update to avoid refreshing completely the view.
    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    using namespace bb::cascades;

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if(!dataModel)
        return;

    const QList<Contact *> *contacts = XMPP::get()->getContacts();
    for(int i = contacts->length()-1 ; i >= 0 ; --i) {
            // remove yourself from the list of contact, and store the info for display
            if(contacts->at(i)->getID().toLower() == from.toLower()) {

                QVariantMap matchMap;
                matchMap["id"] = from;

                QVariantList index = dataModel->find(matchMap);
                QVariantMap updateMap = dataModel->data(index).toMap();
                updateMap["preview"] = message;
                updateMap["name"] = contacts->at(i)->getName();
                updateMap["avatar"] = contacts->at(i)->getAvatar();
                updateMap["presence"] = contacts->at(i)->getPresence();
                updateMap["id"] = from;
                updateMap["read"] = ConversationManager::get()->isAdressee(from);

                QDateTime now = QDateTime::currentDateTime();
                updateMap["timestamp"] = now.toString("hh:mm");

                qDebug() << "Update message: " << dataModel->updateItem(index, updateMap);

                break;
            }
    }
}

void ListContactsController::markRead() {
    ConversationManager::get()->markRead();

    qDebug() << "Mark READ";


    // lazy update to avoid refreshing completely the view.
    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    using namespace bb::cascades;

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if(!dataModel)
        return;

    const QList<Contact *> *contacts = XMPP::get()->getContacts();
    for(int i = contacts->length()-1 ; i >= 0 ; --i) {
            // remove yourself from the list of contact, and store the info for display
       if(ConversationManager::get()->isAdressee(contacts->at(i)->getID())) {

            QVariantMap matchMap;
            matchMap["id"] = contacts->at(i)->getID();

            TimeEvent e = ConversationManager::get()->getPreview();

            QVariantList index = dataModel->find(matchMap);
            QVariantMap updateMap = dataModel->data(index).toMap();
            updateMap["preview"] = e.m_What;
            updateMap["name"] = contacts->at(i)->getName();
            updateMap["avatar"] = contacts->at(i)->getAvatar();
            updateMap["presence"] = contacts->at(i)->getPresence();
            updateMap["id"] = contacts->at(i)->getID();
            updateMap["read"] = 1;

            QDateTime now = QDateTime::currentDateTime();
            QDateTime dt = QDateTime::fromString(e.m_When);
            if(dt.date() == now.date())
                updateMap["timestamp"] = dt.time().toString("hh:mm");
            else
                updateMap["timestamp"] = dt.date().toString();

            qDebug() << "Update message: " << dataModel->updateItem(index, updateMap);

            break;
        }
    }

    ConversationManager::get()->closeConversation();

}

void ListContactsController::updateView() {



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
                              << "avatar"
                              << "preview"
                              << "presence"
                              << "read"
                );
        m_ListView->setDataModel(dataModel);
    }
    dataModel->setGrouping(ItemGrouping::ByFullValue);

    // ----------------------------------------------------------------------------------------------
    // Read login info
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

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    QDateTime now = QDateTime::currentDateTime();

    const QList<Contact *> *contacts = XMPP::get()->getContacts();

    QList<QObject*> datas;
    for(int i = contacts->length()-1 ; i >= 0 ; --i) {
        // remove yourself from the list of contact, and store the info for display
        if(contacts->at(i)->getID().toLower() != m_User.toLower()) {

            TimeEvent e = ConversationManager::get()->getPreview(contacts->at(i)->getID());

            Contact *nc = new Contact;
            nc->setAvatar(contacts->at(i)->getAvatar());
            nc->setName(contacts->at(i)->getName());

            QDateTime time = QDateTime::fromString(e.m_When);
            if(time.date() == now.date())
                nc->setTimestamp(time.time().toString("hh:mm"));
            else
                nc->setTimestamp(time.date().toString());

            nc->setPreview(e.m_What);
            nc->setID(contacts->at(i)->getID());
            nc->setPresence(contacts->at(i)->getPresence());
            nc->setRead(e.m_Read);

            datas.push_back(nc);
        } else {
            setUserName(contacts->at(i)->getName());
            setAvatar(contacts->at(i)->getAvatar());
        }
    }

    dataModel->clear();
    dataModel->insertList(datas);
}
