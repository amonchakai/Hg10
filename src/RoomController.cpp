/*
 * RoomController.cpp
 *
 *  Created on: 1 nov. 2014
 *      Author: pierre
 */


#include "RoomController.hpp"
#include "ConversationManager.hpp"
#include "XMPPService.hpp"

RoomController::RoomController(QObject *parent) : QObject(parent), m_ListView(NULL) {
    m_Avatar = ConversationManager::get()->getAvatar();


}


