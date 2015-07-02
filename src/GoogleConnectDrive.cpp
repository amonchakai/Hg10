/*
 * GoogleConnectDrive.cpp
 *
 *  Created on: 7 juin 2015
 *      Author: pierre
 */

#include "GoogleConnectController.hpp"
#include "Image/HFRNetworkAccessManager.hpp"
#include "ConversationManager.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>

#include <QDir>
#include <QFile>
#include <limits>
#include <QReadWriteLock>
#include <bb/cascades/WebStorage>

#include "DataObjects.h"

#include "PrivateAPIKeys.h"




// -----------------------------------------------------------------------------------------------------------
// This file contain the Google Drive related functions
// It is still part of GoogleConnectController, but the file GoogleConnectController was kept focus on OAUTH and GMAIL API


void GoogleConnectController::putFile(const QString &path) {
    uploadFile(path);
}

void GoogleConnectController::uploadFile(const QString &path, const QString &folder) {
    QFile file( path);

    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadOnly))
        return;

    if(!m_Settings->contains("DriveHomeFolderID")) {
        m_LastUploadedFile = path;
        m_CreatingRootDir = true;
        createFolder();
        return;
    }

    QString name = path.mid(path.lastIndexOf("/")+1);
    QString extension = getContentTypeByExtension(path.mid(path.lastIndexOf(".")+1));


    QString boundary = "---------------------------193971182219750";
    QByteArray datas(QString("\r\n\r\n--" + boundary + "\r\n").toAscii());
    datas += QString("Content-Type: application/json; charset=UTF-8\r\n\r\n").toAscii();
    datas += QString("{\r\n").toAscii();
    datas += QString("\"title\": \"" + name + "\",\r\n").toAscii();
    datas += QString("\"mimeType\": \"" + extension + "\",\r\n").toAscii();
    if(folder.isEmpty())
        datas += (QString("\"parents\": [{\r\n")
                         +  "\"kind\": \"drive#fileLink\",\r\n"
                         +  "\"id\": \"" + m_Settings->value("DriveHomeFolderID").toString() + "\"\r\n"
                   + "}]\r\n").toAscii();
    else
        datas += (QString("\"parents\": [{\r\n")
                         +  "\"kind\": \"drive#fileLink\",\r\n"
                         +  "\"id\": \"" + folder + "\"\r\n"
                   + "}]\r\n").toAscii();

    datas += QString("}\r\n\r\n").toAscii();

    datas += QString("--" + boundary + "\r\n").toAscii();
    datas += QString("Content-Type:" + extension + "\r\n\r\n").toAscii();

    datas += file.readAll();
    datas += QString("\r\n").toAscii();
    datas += QString("--" + boundary + "--").toAscii();

    QNetworkRequest request(QUrl("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart"));
    request.setRawHeader("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/related; boundary=\"" + boundary + "\"");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QString::number(datas.length()).toAscii());
    file.close();

    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, datas);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkUploadReply()));
    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploading(qint64, qint64)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);


}

void GoogleConnectController::createFolder(const QString &name, const QString &root) {
    QByteArray datas;
    datas += QString("{\r\n").toAscii();
    datas += QString("\"title\": \"" + name + "\",\r\n").toAscii();
    datas += QString("\"mimeType\": \"application/vnd.google-apps.folder\"\r\n").toAscii();
    if(!root.isEmpty())
        datas += QString(",\"parents\": [{\"id\":\"" + root + "\"}]");
    else
        if(!m_CurrentDir.isEmpty())
            datas += QString(",\"parents\": [{\"id\":\"" + m_CurrentDir + "\"}]");
    datas += QString("}\r\n\r\n").toAscii();

    QNetworkRequest request(QUrl("https://www.googleapis.com/drive/v2/files"));
    request.setRawHeader("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, datas);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkCreateHomeReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void GoogleConnectController::checkCreateHomeReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QRegExp homeFolderId("\"id\": \"([^\"]+)\",");
                if(homeFolderId.indexIn(response) != -1) {
                    if(m_CreatingRootDir) {
                        setHomeFolder(homeFolderId.cap(1));
                        putFile(m_LastUploadedFile);
                    } else {
                        emit folderCreated();
                    }
                }

            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}

void GoogleConnectController::checkUploadReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QRegExp fileID("\"id\": \"([^\"]+)\",");
                if(fileID.indexIn(response) != -1) {
                    m_LastUploadedFile = fileID.cap(1);
                }

                QRegExp distUrl("\"alternateLink\": \"([^\"]+)\",");
                if(distUrl.indexIn(response) != -1) {
                    m_DistUrl = distUrl.cap(1);
                }


                emit uploaded();
            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}

void GoogleConnectController::uploading(qint64 status, qint64 total) {
    emit uploading(100 * status / total);
}

QString GoogleConnectController::getContentTypeByExtension(const QString &extension) {
    QString contentType;

    if(extension == "doc" || extension == "docx") contentType = "application/msword";
    if(extension == "xls") contentType = "application/vnd.ms-excel";
    if(extension == "ppt" || extension == "pptx") contentType = "application/vnd.ms-powerpoint";
    if(extension == "pdf") contentType = "application/pdf";
    if(extension == "exe") contentType = "application/x-msdos-program";
    if(extension == "rar") contentType = "application/rar";
    if(extension == "png") contentType = "image/png";
    if(extension == "png") contentType = "application/rtf";
    if(extension == "tar") contentType = "application/x-tar";
    if(extension == "zip") contentType = "application/zip";
    if(extension == "") contentType = "";
    if(extension == "jpeg" || extension == "jpg" || extension == "jpe") contentType = "image/jpeg";
    if(extension == "gif") contentType = "image/gif";
    if(extension == "wav") contentType = "application/x-wav";
    if(extension == "tiff" || extension == "tif") contentType = "image/tiff";
    if(extension == "txt" || extension == "cpp" || extension == "h" || extension == "c") contentType = "text/plain";
    if(extension == "mpeg" || extension == "mpg" || extension == "mpe" ) contentType = "video/mpeg";
    if(extension == "qt" || extension == "mov") contentType = "video/quicktime";
    if(extension == "qvi") contentType = "video/x-msvideo";
    if(extension == "video/x-sgi-movie") contentType = "movie";
    if(extension == "exe") contentType = "application/x-msdos-program";

    return contentType;
}

void GoogleConnectController::shareId(const QString &id, const QString &path) {
    qDebug() << "share request!";

    if(!path.isEmpty())
        m_DistUrl = path;

    QByteArray datas;
    datas += QString("{\r\n").toAscii();
    datas += QString("\"role\": \"reader\",\r\n").toAscii();
    datas += QString("\"type\": \"anyone\",\r\n").toAscii();
    datas += QString("\"withLink\": true\r\n").toAscii();
    datas += QString("}\r\n\r\n").toAscii();

    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/drive/v2/files/") + id + "/permissions"));
    request.setRawHeader("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, datas);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyShare()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void GoogleConnectController::share() {
    shareId(m_LastUploadedFile);
}




void GoogleConnectController::checkReplyShare() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                qDebug() << m_DistUrl;

                QRegExp check("anyoneWithLink");
                if(check.indexIn(response) != -1)
                    emit shared(m_DistUrl);


            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}

//------------------------------------------------------------------------------
// google drive stuff

void GoogleConnectController::refresh() {
    getFileList(m_CurrentDir);
}

void GoogleConnectController::getFileList(const QString &directory) {

    QString query;
    if(directory.isEmpty())
        query = "\'root\' in parents";
    else {
        query = "\'" + directory + "\' in parents";
        m_CurrentDir = directory;
    }

    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/drive/v2/files?")
                                    + "q=" + query
                                    + "&key=" + GOOGLE_API_KEY
                                    + "&trashed=false"
                               )
                            );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii());

    m_WithButNoKey = "DIRECTORY_REQUEST:" + directory;

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getFileListReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void GoogleConnectController::search(const QString& key) {
    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/drive/v2/files?")
                                    + "q=title contains \'" + key + "\'"
                                    + "&key=" + GOOGLE_API_KEY
                                    + "&trashed=false"
                               )
                            );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii());

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getFileListReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void GoogleConnectController::popFolder() {
    getFileList(m_ParentID);
}

void GoogleConnectController::getFileListReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                parseFileList(response);
            }

            QRegExp parent("\"selfLink\": \"https://www.googleapis.com/drive/v2/files?q=\'([^\']+)\'");
            if(parent.indexIn(response) != -1) {
                m_ParentID = parent.cap(1);
            } else {
                m_ParentID = "root";
            }

        } else {
            qDebug() << "reply... " << reply->errorString();
            renewToken();
        }

        reply->deleteLater();
    }
}



void GoogleConnectController::parseFileList(const QString &page) {
    QRegExp regexp("\"kind\": \"drive#file\",");
    int pos = 0;
    int lastPos = regexp.indexIn(page, pos);


    if(lastPos == -1) {
        qDebug()<< "empty";
        emit folderEmpty();
        return;
    }

    while((pos = regexp.indexIn(page, lastPos)) != -1) {
        pos += regexp.matchedLength();

        // parse each file individually
        parseFileEntry(page.mid(lastPos, pos-lastPos));

        lastPos = pos;
    }
    parseFileEntry(page.mid(lastPos, pos-lastPos));

}


void GoogleConnectController::parseFileEntry(const QString &entry) {
    QRegExp id("\"id\": \"([^\"]+)\"");
    QRegExp iconLink("\"iconLink\": \"([^\"]+)\",");
    QRegExp title("\"title\": \"([^\"]+)\",");
    QRegExp type ("\"mimeType\": \"([^\"]+)\",");
    QRegExp lastEdit("\"modifiedDate\": \"([^\"]+)\",");
    QRegExp openLink("\"embedLink\": \"([^\"]+)\"");

    QRegExp downloadLink("\"downloadUrl\": \"([^\"]+)\"");

    int pos = id.indexIn(entry);
    if(pos == -1) return;
    pos += id.matchedLength();

    pos = iconLink.indexIn(entry, pos);
    if(pos == -1) return;
    pos += iconLink.matchedLength();

    pos = title.indexIn(entry, pos);
    if(pos == -1) return;
    pos += title.matchedLength();

    pos = type.indexIn(entry, pos);
    if(pos == -1) return;
    pos += type.matchedLength();

    pos = lastEdit.indexIn(entry, pos);
    if(pos == -1) return;
    pos += lastEdit.matchedLength();


    DriveItem *d = new DriveItem(this);
    d->setID(id.cap(1));
    d->setIconLink(iconLink.cap(1));
    d->setTitle(title.cap(1));
    d->setType(type.cap(1));

    int npos = downloadLink.indexIn(entry, pos);
    if(npos != -1) {
        d->setDownloadLink(downloadLink.cap(1));
    } else {
        QRegExp export1("\"application/vnd.openxmlformats-officedocument.wordprocessingml.document\": \"([^\"]+)\"");
        npos = export1.indexIn(entry, pos);
        if(npos != -1) {
            d->setDownloadLink(export1.cap(1));
        } else {
            QRegExp export2("\"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet\": \"([^\"]+)\"");
            npos = export2.indexIn(entry, pos);
            if(npos != -1) {
                d->setDownloadLink(export2.cap(1));
            } else {
                QRegExp export3("\"application/pdf\": \"([^\"]+)\"");
                npos = export3.indexIn(entry, pos);
                if(npos != -1)
                    d->setDownloadLink(export3.cap(1));
            }
        }
    }

    if(openLink.indexIn(entry) != -1) {
        QString link = openLink.cap(1);
        d->setOpenLink(link);
    } else {
        QRegExp alternateLink("\"alternateLink\": \"([^\"]+)\"");
        if(alternateLink.indexIn(entry) != -1) {
            QString link = alternateLink.cap(1);
            d->setOpenLink(link);
        }
    }


    QDateTime date = QDateTime::fromString(lastEdit.cap(1).mid(0,lastEdit.cap(1).length()-1), "yyyy-MM-ddThh:mm:ss.zzz");
    d->setTimestamp(date.toMSecsSinceEpoch());

    emit driveItemLoaded(d);

}

void GoogleConnectController::setHomeFolder(const QString &id) {
    m_Settings->setValue("DriveHomeFolderID", id);
}


void GoogleConnectController::setFileName(const QString &id, const QString &name) {
    QByteArray datas;
    datas += QString("{\r\n").toAscii();
    datas += QString("\"title\": \"" + name + "\"\r\n").toAscii();
    datas += QString("}\r\n\r\n").toAscii();

    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/drive/v2/files/") + id + "?key=" + GOOGLE_API_KEY));
    request.setRawHeader("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->put(request, datas);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyRename()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void GoogleConnectController::checkReplyRename() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                emit uploaded();
            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}



void GoogleConnectController::getOnlineTree(const QString &id, qint64 lastSynch, bool flush) {

    if(flush) {
        mutexGoogleConnect.lockForWrite();
        m_OnlineTree.clear();
        m_LastSynch = lastSynch;
        mutexGoogleConnect.unlock();

    }

    QString query;
    if(id.isEmpty())
        query = "\'root\' in parents";
    else {
        query = "\'" + id + "\' in parents";
    }

    if(lastSynch != 0) {
        QDateTime time = QDateTime::fromMSecsSinceEpoch(lastSynch);

        query += " and (modifiedDate > " + time.toString("YYYY-MM-ddThh:mm:ss") + " or mimeType = \'application/vnd.google-apps.folder\') ";
    }

    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/drive/v2/files?")
                                    + "q=" + query
                                    + "&key=" + GOOGLE_API_KEY
                                    + "&trashed=false"
                               )
                            );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii());

    m_WithButNoKey = "TREE_REQUEST:" + id;

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getBranchFileListReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void GoogleConnectController::getBranchFileListReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QString parentId;
                //qDebug() << response;
                QRegExp parent("\"https://www.googleapis.com/drive/v2/files.q=\'([^\']+)\'");
                if(parent.indexIn(response) != -1) {
                    parentId = parent.cap(1);
                } else {
                    parentId = "";
                }

                parseTree(parentId, response);
            }

        } else {
            qDebug() << "reply... " << reply->errorString();
            renewToken();
        }

        reply->deleteLater();
    }
}

void GoogleConnectController::parseTree(const QString &parent, const QString &page) {
    QRegExp regexp("\"kind\": \"drive#file\",");
    int pos = 0;
    int lastPos = regexp.indexIn(page, pos);


    if(lastPos == -1) {
        //qDebug()<< "empty";
        return;
    }

    while((pos = regexp.indexIn(page, lastPos)) != -1) {
        pos += regexp.matchedLength();

        // parse each file individually
        parseTreeEntry(parent, page.mid(lastPos, pos-lastPos));

        lastPos = pos;
    }
    parseTreeEntry(parent, page.mid(lastPos, pos-lastPos));

}


void GoogleConnectController::parseTreeEntry(const QString &parent, const QString &entry) {
    QRegExp id("\"id\": \"([^\"]+)\"");
    QRegExp iconLink("\"iconLink\": \"([^\"]+)\",");
    QRegExp title("\"title\": \"([^\"]+)\",");
    QRegExp type ("\"mimeType\": \"([^\"]+)\",");
    QRegExp lastEdit("\"modifiedDate\": \"([^\"]+)\",");
    QRegExp openLink("\"embedLink\": \"([^\"]+)\"");

    QRegExp downloadLink("\"downloadUrl\": \"([^\"]+)\"");


    int pos = id.indexIn(entry);
    if(pos == -1) return;
    pos += id.matchedLength();

    pos = iconLink.indexIn(entry, pos);
    if(pos == -1) return;
    pos += iconLink.matchedLength();

    pos = title.indexIn(entry, pos);
    if(pos == -1) return;
    pos += title.matchedLength();

    mutexGoogleConnect.lockForWrite();
    m_OnlineTree[id.cap(1)] = QPair<QString, QString>(parent, title.cap(1));
    mutexGoogleConnect.unlock();

    pos = type.indexIn(entry, pos);
    if(pos == -1) return;
    pos += type.matchedLength();

    if(type.cap(1) == "application/vnd.google-apps.folder") {
        getOnlineTree(id.cap(1), m_LastSynch);
        return;
    }

    pos = lastEdit.indexIn(entry, pos);
    if(pos == -1) return;
    pos += lastEdit.matchedLength();


    DriveItem *d = new DriveItem(this);
    d->setID(id.cap(1));
    d->setIconLink(iconLink.cap(1));
    d->setTitle(title.cap(1));
    d->setType(type.cap(1));

    int npos = downloadLink.indexIn(entry, pos);
    if(npos != -1) {
        d->setDownloadLink(downloadLink.cap(1));
    } else {
        QRegExp export1("\"application/vnd.openxmlformats-officedocument.wordprocessingml.document\": \"([^\"]+)\"");
        npos = export1.indexIn(entry, pos);
        if(npos != -1) {
            d->setDownloadLink(export1.cap(1));
        } else {
            QRegExp export2("\"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet\": \"([^\"]+)\"");
            npos = export2.indexIn(entry, pos);
            if(npos != -1) {
                d->setDownloadLink(export2.cap(1));
            } else {
                QRegExp export3("\"application/pdf\": \"([^\"]+)\"");
                npos = export3.indexIn(entry, pos);
                if(npos != -1)
                    d->setDownloadLink(export3.cap(1));
            }
        }
    }

    if(openLink.indexIn(entry) != -1) {
        QString link = openLink.cap(1);
        d->setOpenLink(link);
    } else {
        QRegExp alternateLink("\"alternateLink\": \"([^\"]+)\"");
        if(alternateLink.indexIn(entry) != -1) {
            QString link = alternateLink.cap(1);
            d->setOpenLink(link);
        }
    }


    QDateTime date = QDateTime::fromString(lastEdit.cap(1).mid(0,lastEdit.cap(1).length()-1), "yyyy-MM-ddThh:mm:ss.zzz");
    d->setTimestamp(date.toMSecsSinceEpoch());


    QString humanUrl;

    mutexGoogleConnect.lockForWrite();
    humanUrl = "";

    QMap<QString, QPair<QString, QString> >::iterator it = m_OnlineTree.find(id.cap(1));

    if(it != m_OnlineTree.end()) {
        humanUrl = it.value().second;
        it = m_OnlineTree.find(it.value().first);
    }

    while(!(it == m_OnlineTree.end())) {
        humanUrl = it.value().second + "/" + humanUrl;
        it = m_OnlineTree.find(it.value().first);
    }

    qDebug() << humanUrl;
    emit onlineTreeLeaf(humanUrl, d);

    mutexGoogleConnect.unlock();

}


