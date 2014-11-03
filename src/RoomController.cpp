/*
 * RoomController.cpp
 *
 *  Created on: 1 nov. 2014
 *      Author: pierre
 */


#include "RoomController.hpp"
#include "ConversationManager.hpp"
#include "XMPPService.hpp"
#include "DataObjects.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <QUuid>
#include <QFile>
#include <QDir>

RoomController::RoomController(QObject *parent) : QObject(parent), m_ListView(NULL) {
    m_Avatar = ConversationManager::get()->getAvatar();


}


void RoomController::createRoom(const QString &participants) {
    GroupChat room;

    int nextIdx = 0;
    int bgIdx = 0;

    // Convert names to user ids
    while(nextIdx != -1) {
        nextIdx = participants.indexOf(";", bgIdx);
        int edIdx;
        if(nextIdx == -1)
            edIdx = participants.length()-1;
        else
            edIdx = nextIdx-1;

        // remove space at the beginning
        while(participants.size() > bgIdx && participants[bgIdx] == QChar(' '))
            ++bgIdx;

        // remove space at the end
        while(edIdx > 0 && participants[edIdx] == QChar(' '))
                --edIdx;

        // get User's name
        QString name = participants.mid(bgIdx, edIdx-bgIdx+1);

        const QList<Contact *> *contacts = XMPP::get()->getContacts();
        for(int i = contacts->length()-1 ; i >= 0 ; --i) {

            // remove yourself from the list of contact, and store the info for display
            if(contacts->at(i)->getID().toLower() != ConversationManager::get()->getUser().toLower() // not yourself
                    && contacts->at(i)->getName().toLower() == name.toLower()  ) { // searched

                room.m_Participants.push_back(contacts->at(i)->getID());
                break;
            }
        }

        bgIdx = nextIdx + 1;
    }

    QString UID = QUuid::createUuid();
    UID = UID.mid(1,UID.length()-2);
    qDebug() << "UID: " << UID;
    room.m_RoomID = "private-chat-" + UID + "@groupchat.google.com";

    // ----------------------------------------------------------------------------

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Rooms");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/" + UID + ".room");

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        stream << room;

        file.close();
    }

    // ----------------------------------------------------------------------------

    XMPP::get()->createRoom(UID);


    // ----------------------------------------------------------------------------

    pushRoom(UID);

}


void RoomController::pushRoom(const QString& uid) {
    if(m_ListView == NULL)
         return;


    GroupChat room;

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Rooms");
    QFile file(directory + "/" + uid + ".room");

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        stream >> room;

        file.close();
    } else {
        return;
    }


    using namespace bb::cascades;
    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());

    if (!dataModel) {
        dataModel = new GroupDataModel(
                    QStringList() << "name"
                                  << "id"
                                  << "avatar"
                    );
        m_ListView->setDataModel(dataModel);
    }



    RenderRoom *r = new RenderRoom;
    r->setID(room.m_RoomID);

    const QList<Contact *> *contacts = XMPP::get()->getContacts();
    for(int j = 0 ; j < room.m_Participants.size() ; ++j) {
        for(int i = 0 ; i < contacts->size() ; ++i) {
            if(contacts->at(i)->getID() == room.m_Participants.at(j)) {
                if(j == 0)
                    r->setName(contacts->at(i)->getName());
                else
                    r->setName(r->getName() + "; " + contacts->at(i)->getName());
                break;
            }
        }
    }

    r->setAvatar("asset:///images/chat_room.png");

    dataModel->insert(r);


}

void RoomController::updateView() {

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Rooms");
    if (QFile::exists(directory)) {
        QDir dir(directory);
        dir.setNameFilters(QStringList() << "*.room");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList()) {

            pushRoom(dirFile.mid(0,dirFile.length()-5));
        }
    }
}


void RoomController::deleteRoom(const QString &roomId) {

    QString fileID = roomId.mid(13, roomId.length()-34);

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Rooms");
    if (QFile::exists(directory + "/" + fileID + ".room")) {
        QDir dir(directory);
        dir.remove(fileID + ".room");
    }

}

