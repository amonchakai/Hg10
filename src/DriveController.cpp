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
#include <bb/system/SystemListDialog>
#include <bb/system/Clipboard>
#include <QCryptographicHash>


#include <QFile>

#include "ConversationManager.hpp"

DriveController::DriveController(QObject *parent) : QObject(parent), m_ListView(NULL), m_listdialog(NULL) {
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

    check = QObject::connect(m_Google, SIGNAL(onlineTreeLeaf(QString , DriveItem *)), this, SLOT(diveSynchPushLeaf(QString , DriveItem *)));
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

QString DriveController::convertMStoStr(qint64 ms) {
    return QDateTime::fromMSecsSinceEpoch(ms).toString(Qt::SystemLocaleShortDate);
}

void DriveController::sortKey() {
    using namespace bb::cascades;
    using namespace bb::system;

    // Create a SystemListDialog with these characteristics:
    // The "confirmLabel" (OK button) is set to "My favorite"
    // The "cancelLabel" (CANCEL button) is set to "Cancel"
    // This dialog box doesn't have a custom button


    if(m_listdialog == NULL) {
        m_listdialog = new SystemListDialog(tr("OK"), tr("Cancel"));

        m_listdialog->setTitle("Sort files by");
        m_listdialog->appendItem("name");
        m_listdialog->appendItem("type");
        m_listdialog->appendItem("date");

        bool success = connect(m_listdialog,
             SIGNAL(finished(bb::system::SystemUiResult::Type)),
             this,
             SLOT(onDialogSortingKeyFinished(bb::system::SystemUiResult::Type)));

        if (success) {
            // Signal was successfully connected
            // Now show the dialog box in your UI

            m_listdialog->show();
        } else {
            // Failed to connect to signal

            m_listdialog->deleteLater();
        }
    } else {
        m_listdialog->show();
    }

}

void DriveController::onDialogSortingKeyFinished(bb::system::SystemUiResult::Type result) {

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());


    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {

        const QList<int> &indices = m_listdialog->selectedIndices();
        QStringList keys;

        switch(indices[0]) {
            case 0:
                keys.push_back("title");
                break;
            case 1:
                keys.push_back("type");
                break;
            case 2:
                keys.push_back("timestamp");
                break;
        }

        if(!keys.isEmpty()) {
            QSettings settings("Amonchakai", "Hg10");
            settings.setValue("DriveSortKey", keys.last());
        }


        dataModel->setSortingKeys(keys);

    }
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

    QSettings settings("Amonchakai", "Hg10");
    QStringList keys = dataModel->sortingKeys();
    if(!keys.isEmpty() && keys.last() != settings.value("DriveSortKey", "type").toString()) {
        keys.clear();
        keys.push_back(settings.value("DriveSortKey", "type").toString());
        dataModel->setSortingKeys(keys);
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

    if(location.isEmpty()) {
        FilePicker* picker = qobject_cast<FilePicker*>(sender());
        picker->deleteLater();
        return;
    }

    m_Mutex.lockForWrite();
    m_Downloads.push_back(QPair<QString, QString>(m_SelectedItemForSharing, location.first()));
    m_Mutex.unlock();

    authentifiedDownload(m_SelectedItemForSharing);

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
                m_Mutex.lockForWrite();
                int selectedDownload;
                for(int i = 0 ; i < m_Downloads.length() ; ++i) {
                    if(m_Downloads.at(i).first == reply->url().toString()) {
                        selectedDownload = i;
                        break;
                    }
                }

                QFile file(m_Downloads.at(selectedDownload).second);
                file.open(QIODevice::WriteOnly);
                file.write(reply->readAll());
                file.close();

                m_Downloads.removeAt(selectedDownload);

                m_Mutex.unlock();

                bb::system::SystemToast *toast = new bb::system::SystemToast(this);
                toast->setBody(tr("Download completed"));
                toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
                toast->show();

            }

        } else {
            qDebug() << "reply... " << reply->errorString();

            // remove case of failure...
            m_Mutex.lockForWrite();
            int selectedDownload = 0;
            for(int i = 0 ; i < m_Downloads.length() ; ++i) {
                if(m_Downloads.at(i).first == reply->url().toString()) {
                    selectedDownload = i;
                    break;
                }
            }
            if(m_Downloads.size() > 0)
                m_Downloads.removeAt(selectedDownload);
            m_Mutex.unlock();
        }

        reply->deleteLater();
    }


    // download all the files in the stack
    m_Mutex.lockForWrite();
    if(m_Downloads.size() > 0) {
        authentifiedDownload(m_Downloads.first().first);
    }

    m_Mutex.unlock();
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


void DriveController::synchronize(const QString &id, const QString &title) {
    using namespace bb::cascades;
    using namespace bb::cascades::pickers;

    m_SelectedItemForSharing = id;
    FilePicker* filePicker = new FilePicker(FileType::Document, 0, QStringList(), QStringList(), QStringList(title));
    filePicker->setMode(FilePickerMode::Saver);


    bool success = QObject::connect(filePicker, SIGNAL(fileSelected(const QStringList &)), this, SLOT(onPromptFinishedSynchLocation(const QStringList &)));
    success = QObject::connect(filePicker, SIGNAL(canceled()), this, SLOT(onPromptFinishedDownloadLocationCanceled()));

    if (success) {
        filePicker->open();
     } else {
         filePicker->deleteLater();
    }

}

void DriveController::onPromptFinishedSynchLocation(const QStringList &location) {
    using namespace bb::cascades;
    using namespace bb::cascades::pickers;

    if(location.isEmpty()) {
        FilePicker* picker = qobject_cast<FilePicker*>(sender());
        picker->deleteLater();
        return;
    }

    loadSynchMap();
    m_SynchMap[m_SelectedItemForSharing] = location.first();
    saveSynchMap();

    FilePicker* picker = qobject_cast<FilePicker*>(sender());
    picker->deleteLater();
}


void DriveController::saveSynchMap() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }

    QFile file(directory + "/SynchMap.txt");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);

        for(QMap<QString, QString>::iterator it = m_SynchMap.begin(); it != m_SynchMap.end() ; ++it) {
            stream << it.key() << it.value();
        }

        file.close();
    }
}

void DriveController::loadSynchMap() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
    QFile file(directory + "/SynchMap.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);

        m_SynchMap.clear();

        QString key, value;
        stream >> key;
        stream >> value;

        while(!key.isEmpty() && !value.isEmpty()) {
            m_SynchMap[key] = value;

            stream >> key;
            stream >> value;
        }

        file.close();
    }
}

void DriveController::updateSynch() {
    loadSynchMap();
    if(m_SynchMap.begin() != m_SynchMap.end())
        getOnlineTree(m_SynchMap.begin().key());
}

void DriveController::getOnlineTree(const QString &id, qint64 lastSynch) {
    m_Google->getOnlineTree(id, lastSynch, true);
}

void DriveController::diveSynchPushLeaf(QString url, DriveItem *item) {
    QString localFile = m_SynchMap.begin().value() + "/" + url;
    QString directory = localFile.mid(0,localFile.lastIndexOf("/"));


    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
        qDebug() << "create folder: " << directory;
    }

    if (!QFile::exists(localFile)) {

        m_Mutex.lockForWrite();
        m_Downloads.push_back(QPair<QString, QString>(item->getDownloadLink(), localFile));

        if(m_Downloads.size() == 1) {
            authentifiedDownload(m_Downloads.first().first);
        }
        m_Mutex.unlock();

    } else {
        QFileInfo info(localFile);

        bool needUpdate = false;
        QFile file(localFile);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream stream(&file);

            qint64 length = file.size();

            char *temp = new char[length];
            stream.readRawData (temp, length);



            QCryptographicHash hasher(QCryptographicHash::Md5);
            hasher.addData(temp);

            QByteArray hash = hasher.result();
            for(int i = 0 ; i < hash.size() & !needUpdate ; ++i) {
                needUpdate = hash[i] == item->getHash().toLocal8Bit()[i];
            }

            delete[] temp;
        }

        if(item->getTimestamp() > info.lastModified().toMSecsSinceEpoch()) {
            m_Mutex.lockForWrite();
            m_Downloads.push_back(QPair<QString, QString>(item->getDownloadLink(), localFile));
            if(m_Downloads.size() == 1) {
                authentifiedDownload(m_Downloads.first().first);
            }
            m_Mutex.unlock();
        }

    }

    delete item;

}


void DriveController::authentifiedDownload(const QString &url) {
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", m_Google->getAuthorizationCode().toAscii());

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkDownload()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
