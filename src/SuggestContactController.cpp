/*
 * SuggestContact.cpp
 *
 *  Created on: 1 nov. 2014
 *      Author: pierre
 */

#include "SuggestContactController.hpp"
#include "DataObjects.h"
#include "XMPPService.hpp"
#include "ConversationManager.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>


SuggestContactController::SuggestContactController(QObject *parent) : QObject(parent), m_ListView(NULL) {

}


void SuggestContactController::suggest(const QString &text) {

    QString searchKey;
    int i = text.lastIndexOf(";");
    if(i == -1) {
        searchKey = text;
    } else {
        searchKey = text.mid(i+1);
    }
    searchKey.replace(" ", "");

    if(searchKey.length() < 3)
        return;

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
                              << "avatar"
                );
        m_ListView->setDataModel(dataModel);
    }

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    const QList<Contact *> *contacts = XMPP::get()->getContacts();
    QList<QObject*> datas;
    for(int i = contacts->length()-1 ; i >= 0 ; --i) {

        // remove yourself from the list of contact, and store the info for display
        if(contacts->at(i)->getID().toLower() != ConversationManager::get()->getUser().toLower() // not yourself
                && contacts->at(i)->getName().mid(0, searchKey.length()).toLower() == searchKey.toLower()  // searched
         ) {

            Contact *nc = new Contact;
            nc->setAvatar(contacts->at(i)->getAvatar());
            nc->setName(contacts->at(i)->getName());
            nc->setID(contacts->at(i)->getID());

            datas.push_back(nc);

        }
    }

    dataModel->clear();
    dataModel->insertList(datas);
}

QString SuggestContactController::addParticipant(const QString &text, const QString &name) {
    int i = text.lastIndexOf(";");

    if(i == -1) {
        return name + "; ";
    }

    return text.mid(0, i+1) + " " + name + ";" ;

}




