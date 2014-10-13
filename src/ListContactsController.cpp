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

ListContactsController::ListContactsController(QObject *parent) : QObject(parent),
    m_ListView(NULL) {

    bool check = connect(XMPP::get(), SIGNAL(contactReceived()), this, SLOT(updateView()));
    Q_ASSERT(check);
    Q_UNUSED(check);

}


void ListContactsController::updateContacts() {
    //qDebug() << presence.
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


    const QList<Contact *> *contacts = XMPP::get()->getContacts();

    QList<QObject*> datas;
    for(int i = contacts->length()-1 ; i >= 0 ; --i) {
        // remove yourself from the list of contact, and store the info for display
        if(contacts->at(i)->getID().toLower() != m_User.toLower()) {

            Contact *nc = new Contact;
            nc->setAvatar(contacts->at(i)->getAvatar());
            nc->setName(contacts->at(i)->getName());
            nc->setTimestamp(contacts->at(i)->getTimestamp());
            nc->setID(contacts->at(i)->getID());

            datas.push_back(nc);
        } else {
            setUserName(contacts->at(i)->getName());
            setAvatar(contacts->at(i)->getAvatar());
        }
    }

    dataModel->clear();
    dataModel->insertList(datas);
}
