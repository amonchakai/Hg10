/*
 * GoogleConnectPicassa.cpp
 *
 *  Created on: 5 juil. 2015
 *      Author: pierre
 */

#include "GoogleConnectController.hpp"
#include "Image/HFRNetworkAccessManager.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>


void GoogleConnectController::getPictureFromLink(const QString& user_id, const QString &picture_id) {

    mutexGoogleConnect.lockForWrite();
    m_PictureIds.push_back(picture_id);
    mutexGoogleConnect.unlock();


    QString query("https://picasaweb.google.com/data/feed/api/user/" + user_id);
    QString album = getAlbumId(user_id);

    if(!album.isEmpty())
        query += "/albumid/" + album;


    // get the picture list from this album (and now, I just need to look for the picture id which was provided in the url)
    QNetworkRequest request(QUrl(QString(query)
                + ""
                )
            );

    // request.setRawHeader("Authorization", ("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii()));
    request.setRawHeader("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii());
    request.setRawHeader("GData-Version", "2");

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok;

    if(album.isEmpty())
        ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyAlbumsList()));
    else
        ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyAlbumsContent()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

QString GoogleConnectController::getAlbumId(const QString& user_id) {
    QString album;

    QString directory = QDir::homePath() + QLatin1String("/Picassa/");
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    } else {
        QFile file(directory + "/" + user_id + ".log");

        if (file.open(QIODevice::ReadOnly)) {

            QTextStream stream(&file);
            stream >> album;
            file.close();
        }
    }

    return album;
}


void GoogleConnectController::checkReplyAlbumsList() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QRegExp matchAlbum("<id>https://picasaweb.google.com/data/entry/user/([0-9]+)/albumid/([0-9]+)</id>");

                int pos = 0;

                QList<QString> albums;
                QString user_id;
                while((pos = matchAlbum.indexIn(response, pos)) != -1) {
                    user_id = matchAlbum.cap(1);
                    albums.push_back(matchAlbum.cap(2));
                    pos += matchAlbum.matchedLength();
                }

                QString directory = QDir::homePath() + QLatin1String("/Picassa/");
                if (!QFile::exists(directory)) {
                    QDir dir;
                    dir.mkpath(directory);
                }

                QFile file(directory + "/" + user_id + ".log");

                if (file.open(QIODevice::WriteOnly)) {
                    QTextStream stream(&file);
                    for(int i = 0 ; i < albums.length() ; ++i)
                        stream << albums.at(i) << "\n";
                    file.close();
                }




                QString query("https://picasaweb.google.com/data/feed/api/user/" + user_id);
                query += "/albumid/" + albums.first();


                // get the picture list from this album (and now, I just need to look for the picture id which was provided in the url)
                QNetworkRequest request(QUrl(QString(query)
                            + ""
                            )
                        );

                // request.setRawHeader("Authorization", ("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii()));
                request.setRawHeader("Authorization", ("Bearer " + m_Settings->value("access_token").value<QString>()).toAscii());
                request.setRawHeader("GData-Version", "2");

                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

                QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
                bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyAlbumsContent()));
                Q_ASSERT(ok);
                Q_UNUSED(ok);

            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}


void GoogleConnectController::checkReplyAlbumsContent() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                mutexGoogleConnect.lockForWrite();

                for(int i = 0 ; i < m_PictureIds.size() ; ++i) {
                    QString imageIdStr = m_PictureIds.at(i);
                    QRegExp imageId(imageIdStr);
                    int pos = imageId.indexIn(response);
                    if(pos != -1) {
                        m_PictureIds.removeAt(i);
                        --i;
                        if(i < 0) i = 0;

                        QRegExp url("src=\'(https://lh3.googleusercontent.com[^\']+)'/>");
                        if(url.indexIn(response, pos) != -1) {
                            qDebug() << "found! : " << url.cap(1);
                            emit picasaImageFound(imageIdStr, url.cap(1));
                        }
                    }
                }

                mutexGoogleConnect.unlock();

            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}



