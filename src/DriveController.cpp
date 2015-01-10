/*
 * DriveController.cpp
 *
 *  Created on: 22 déc. 2014
 *      Author: pierre
 */


#include "DriveController.hpp"
#include "GoogleConnectController.hpp"
#include "DataObjects.h"
#include "Image/WebResourceManager.h"
#include "Image/HFRNetworkAccessManager.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/pickers/FilePicker>
#include <bb/system/SystemToast>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemDialog>
#include <bb/system/Clipboard>

#include <QFile>

#include "ConversationManager.hpp"

DriveController::DriveController(QObject *parent) : QObject(parent), m_ListView(NULL) {
    //m_Google = new GoogleConnectController(this);
    m_Google = ConversationManager::get()->getFileTransfert();
    if(m_Google == NULL)
        m_Google = new GoogleConnectController(this);

    bool check = QObject::connect(m_Google, SIGNAL(driveItemLoaded(DriveItem *)), this, SLOT(pushDriveItem(DriveItem *)));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = QObject::connect(m_Google, SIGNAL(folderEmpty()), this, SLOT(notifyEmptyFolder()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = QObject::connect(m_Google, SIGNAL(folderCreated()), this, SLOT(updateView()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = QObject::connect(WebResourceManager::get(), SIGNAL(onImageReady(const QString &, const QString &)), this, SLOT(onImageReady(const QString &, const QString &)));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = QObject::connect(m_Google, SIGNAL(uploading(int)), this, SLOT(fowardUploading(int)));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = QObject::connect(m_Google, SIGNAL(uploaded()), this, SLOT(updateView()));
    Q_ASSERT(check);
    Q_UNUSED(check);
}


void DriveController::getFileList() {
    m_Mutex.lockForWrite();
    m_DriveItems.clear();
    m_Mutex.unlock();

    m_Google->getFileList();

}

void DriveController::notifyEmptyFolder() {
    emit complete();
}

void DriveController::pushDriveItem(DriveItem *item) {

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }


    m_Mutex.lockForWrite();
    m_DriveItems.push_back(item);
    m_Mutex.unlock();

    WebResourceManager::get()->getImage(item->getIconLink());

}

void DriveController::onImageReady(const QString &url, const QString &diskPath) {

    if(diskPath == "loading")
        return;

    if(url[0] == '/')
        return;

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
                QStringList() << "id"
                              << "iconLink"
                              << "title"
                              << "type"
                              << "timestamp"
                );
        m_ListView->setDataModel(dataModel);
    }

    m_Mutex.lockForWrite();
    for(int i = 0 ; i < m_DriveItems.length() ; ++i) {
        if(m_DriveItems.at(i)->getIconLink() == url) {
            m_DriveItems.at(i)->setIconLink(diskPath);
            dataModel->insert(m_DriveItems.at(i));
            break;
        }
    }
    m_Mutex.unlock();

    emit complete();

}

void DriveController::openForSharing(const QString &id, const QString &type) {

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());

    if(type == "application/vnd.google-apps.folder") {
        if(dataModel != NULL)
            dataModel->clear();

        m_Google->getFileList(id);
    } else {
        using namespace bb::cascades;
        using namespace bb::system;

        SystemDialog *dialog = new SystemDialog("Yes", "No");

        dialog->setTitle("Share");
        dialog->setBody("Do you want to share this document?");

        bool success = connect(dialog,
             SIGNAL(finished(bb::system::SystemUiResult::Type)),
             this,
             SLOT(onPromptFinishedShareFile(bb::system::SystemUiResult::Type)));

        if (success) {
            // Signal was successfully connected.
            // Now show the dialog box in your UI.
            m_SelectedItemForSharing = id;
            dialog->show();
        } else {
            // Failed to connect to signal.
            // This is not normal in most cases and can be a critical
            // situation for your app! Make sure you know exactly why
            // this has happened. Add some code to recover from the
            // lost connection below this line.
            dialog->deleteLater();
        }

    }

}

void DriveController::onPromptFinishedShareFile(bb::system::SystemUiResult::Type result) {
    using namespace bb::cascades;
    using namespace bb::system;

    qDebug() << "onPromptFinishedShareFile " << result;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        SystemDialog* prompt = qobject_cast<SystemDialog*>(sender());
        if(prompt != NULL) {

            QString url;
            m_Mutex.lockForRead();
            for(int i = 0 ; i < m_DriveItems.length() ; ++i)
                if(m_DriveItems.at(i)->getID() == m_SelectedItemForSharing) {
                    url = m_DriveItems.at(i)->getOpenLink();
                    break;
                }
            m_Mutex.unlock();

            m_Google->shareId(m_SelectedItemForSharing, url);

            prompt->deleteLater();
        }
    }
}

void DriveController::open(const QString &id, const QString &type) {
    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());

    if(type == "application/vnd.google-apps.folder") {
        if(dataModel != NULL)
            dataModel->clear();

        m_Google->getFileList(id);
    } else {

        QString link;
        m_Mutex.lockForRead();
        for(int i = 0 ; i < m_DriveItems.length() ; ++i) {
            if(m_DriveItems.at(i)->getID() == id) {
                link = m_DriveItems.at(i)->getOpenLink();
                break;
            }
        }
        m_Mutex.unlock();

        if(!link.isEmpty())
            emit pushOpenLink(link);
        else {
            bb::system::SystemToast *toast = new bb::system::SystemToast(this);

            toast->setBody(tr("Cannot open this file..."));
            toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
            toast->show();
        }
    }

}

void DriveController::pop() {
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());

    if(dataModel != NULL) {
        m_DriveItems.clear();
        dataModel->clear();
    }

    m_Google->popFolder();
}

void DriveController::updateView() {
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());

    if(dataModel != NULL) {
        m_DriveItems.clear();
        dataModel->clear();
    }

    m_Google->refresh();
}

void DriveController::createNewFolder() {
    using namespace bb::cascades;
    using namespace bb::system;

    SystemPrompt *prompt = new SystemPrompt();
    prompt->setTitle(tr("Create new folder"));
    prompt->setDismissAutomatically(true);
    prompt->inputField()->setEmptyText(tr("name..."));


    bool success = QObject::connect(prompt,
        SIGNAL(finished(bb::system::SystemUiResult::Type)),
        this,
        SLOT(onPromptFinishedCreateFolder(bb::system::SystemUiResult::Type)));

    if (success) {
        prompt->show();
     } else {
        prompt->deleteLater();
    }
}

void DriveController::onPromptFinishedCreateFolder(bb::system::SystemUiResult::Type result) {
    using namespace bb::cascades;
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        SystemPrompt* prompt = qobject_cast<SystemPrompt*>(sender());
        if(prompt != NULL) {
            m_Google->createFolder(prompt->inputFieldTextEntry());

            prompt->deleteLater();
        }
    }
}

void DriveController::setHomeFolder(const QString &id) {
    m_Google->setHomeFolder(id);

    bb::system::SystemToast *toast = new bb::system::SystemToast(this);

    toast->setBody(tr("Home folder set!"));
    toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
    toast->show();
}

void DriveController::copyShareLink(const QString& id, const QString &link) {
    m_Google->shareId(id);

    bb::system::Clipboard clipboard;
    clipboard.clear();
    clipboard.insert("text/plain", link.toAscii());

    bb::system::SystemToast *toast = new bb::system::SystemToast(this);
    toast->setBody(tr("Link copied to clipboard"));
    toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
    toast->show();
}

void DriveController::renameFile(const QString &id, const QString &title) {
    using namespace bb::cascades;
    using namespace bb::system;

    SystemPrompt *prompt = new SystemPrompt();
    prompt->setTitle(tr("Rename"));
    prompt->setDismissAutomatically(true);
    prompt->inputField()->setEmptyText(tr("name..."));
    prompt->inputField()->setDefaultText(title);


    bool success = QObject::connect(prompt,
        SIGNAL(finished(bb::system::SystemUiResult::Type)),
        this,
        SLOT(onPromptFinishedRenameFile(bb::system::SystemUiResult::Type)));

    if (success) {
        m_SelectedItemForSharing = id;
        prompt->show();
     } else {
        prompt->deleteLater();
    }
}

void DriveController::downloadFile(const QString &fileUrl, const QString &title) {
    using namespace bb::cascades;
    using namespace bb::cascades::pickers;

    m_SelectedItemForSharing = fileUrl;
    FilePicker* filePicker = new FilePicker(FileType::Document, 0, QStringList(), QStringList(), QStringList(title));
    filePicker->setMode(FilePickerMode::Saver);


    bool success = QObject::connect(filePicker, SIGNAL(fileSelected(const QStringList &)), this, SLOT(onPromptFinishedDownloadLocation(const QStringList &)));
    success = QObject::connect(filePicker, SIGNAL(canceled()), this, SLOT(onPromptFinishedDownloadLocationCanceled()));

    if (success) {
        filePicker->open();
     } else {
         filePicker->deleteLater();
    }
}

void DriveController::onPromptFinishedDownloadLocation(const QStringList &location) {
    using namespace bb::cascades;
    using namespace bb::cascades::pickers;

    if(location.isEmpty())
        return;

    m_DowloadLocation = location.first();


    QNetworkRequest request(m_SelectedItemForSharing);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkDownload()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);


    FilePicker* picker = qobject_cast<FilePicker*>(sender());
    picker->deleteLater();
}

void DriveController::onPromptFinishedDownloadLocationCanceled() {
    using namespace bb::cascades;
    using namespace bb::cascades::pickers;

    FilePicker* picker = qobject_cast<FilePicker*>(sender());
    picker->deleteLater();
}

void DriveController::checkDownload() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                QFile file(m_DowloadLocation);
                file.open(QIODevice::WriteOnly);
                file.write(reply->readAll());
                file.close();

                bb::system::SystemToast *toast = new bb::system::SystemToast(this);
                toast->setBody(tr("Download completed"));
                toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
                toast->show();

            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}

void DriveController::upload(const QString &path) {
    m_Google->uploadFile(path, m_Google->getCurrentPath());
}

void DriveController::fowardUploading(int status) {
    emit uploading(status);
}

void DriveController::askName() {
    using namespace bb::cascades;
    using namespace bb::system;

    SystemPrompt *prompt = new SystemPrompt();
    prompt->setTitle(tr("Recording\'s name"));
    prompt->setDismissAutomatically(true);
    prompt->inputField()->setEmptyText(tr("name..."));


    bool success = QObject::connect(prompt,
        SIGNAL(finished(bb::system::SystemUiResult::Type)),
        this,
        SLOT(onPromptFinishedChooseName(bb::system::SystemUiResult::Type)));

    if (success) {
        prompt->show();
     } else {
        prompt->deleteLater();
    }

}

void DriveController::onPromptFinishedChooseName(bb::system::SystemUiResult::Type result) {
    using namespace bb::cascades;
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        SystemPrompt* prompt = qobject_cast<SystemPrompt*>(sender());
        if(prompt != NULL) {
            QString directory = QDir::homePath() + QLatin1String("/ApplicationData/AudioMessages/");
            m_AudioName = directory + prompt->inputFieldTextEntry() + ".m4a";
            emit startRecording();
            prompt->deleteLater();
        }
    }
}


void DriveController::onPromptFinishedRenameFile(bb::system::SystemUiResult::Type result) {
    using namespace bb::cascades;
    using namespace bb::system;

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {
        SystemPrompt* prompt = qobject_cast<SystemPrompt*>(sender());
        if(prompt != NULL) {
            m_Google->setFileName(m_SelectedItemForSharing, prompt->inputFieldTextEntry());
            prompt->deleteLater();
        }
    }
}


