/*
 * CustomizationController.cpp
 *
 *  Created on: 1 févr. 2015
 *      Author: pierre
 */

#include "CustomizationController.hpp"
#include <QDir>
#include <QFile>
#include <QList>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/system/SystemDialog>
#include <bb/system/SystemToast>

#include "DataObjects.h"
#include "QXmppVCardIq.h"

CustomizationController::CustomizationController(QObject *parent) : QObject(parent), m_ListView(NULL) {

}


void CustomizationController::updateView() {

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
                QStringList() << "userName"
                              << "userId"
                              << "wallpaper"
                );
        m_ListView->setDataModel(dataModel);
    }


    // ----------------------------------------------------------------------------------------------


    QList<QObject*> datas;

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Customization");
    if (QFile::exists(directory)) {
        QDir dir(directory);
        dir.setNameFilters(QStringList() << "*.xml");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList()) {

            QString userName;
            if(dirFile == "default.xml") {
                userName = tr("Default");
            } else {

                // -------------------------------------------------------------
                // get vCard from file
                QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");
                QFile file(vCardsDir + "/" + dirFile);

                QDomDocument doc("vCard");
                if (!file.open(QIODevice::ReadOnly))
                    return;
                doc.setContent(&file);
                file.close();

                QXmppVCardIq vCard;
                vCard.parse(doc.documentElement());

                userName = vCard.fullName();

            }

            WallpaperItem *item = new WallpaperItem;
            item->setUserId(dirFile.mid(0, dirFile.length()-4));
            item->setUserName(userName);

            QFile file(directory + "/" + dirFile);
            if (file.open(QIODevice::ReadOnly)) {
                QTextStream stream(&file);
                QString themeSettings = stream.readAll();

                QRegExp wallpaper("<wallpaper url=\"([^\"]+)\"");
                if(wallpaper.indexIn(themeSettings) != -1) {
                    item->setWallpaper("file://" + wallpaper.cap(1));

                }

                file.close();
            }

            datas.push_back(item);
        }
    }


    dataModel->clear();
    dataModel->insertList(datas);

}

void CustomizationController::createDefault() {
    using namespace bb::cascades;
    using namespace bb::system;

    SystemDialog *dialog = new SystemDialog(tr("OK"), tr("Cancel"));

    dialog->setTitle(tr("Create new theme"));
    dialog->setBody(tr("This will create a new default theme. A per-user theme can be defined using the wallpaper option in the chat view."));

    bool success = connect(dialog,
         SIGNAL(finished(bb::system::SystemUiResult::Type)),
         this,
         SLOT(onPromptFinishedCreateTheme(bb::system::SystemUiResult::Type)));

    if (success) {
        dialog->show();
    } else {
        dialog->deleteLater();
    }
}


void CustomizationController::onPromptFinishedCreateTheme(bb::system::SystemUiResult::Type result) {

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Customization");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }


    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        QFile file(directory + "/default.xml");

        if(file.exists()) {
            bb::system::SystemToast *toast = new bb::system::SystemToast(this);

           toast->setBody(tr("A default theme already exists. Please edit this one or delete it."));
           toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
           toast->show();
           return;
        }

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << "<root>";
            stream << QString("<wallpaper url=\"") + "" + "\" />";
            stream << "</root>";
            file.close();
        }

        updateView();

    }

}


void CustomizationController::deleteCustom(const QString &id) {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Customization");
    if (!QFile::exists(directory)) {
        return;
    }

    {
        QFile file(directory + "/" + id + ".xml");
        file.remove();
    }

    {
        QFile file(directory + "/" + id + ".css");
        file.remove();
    }


}






