/*
 * DropBoxConnectController.cpp
 *
 *  Created on: 26 oct. 2014
 *      Author: pierre
 */

#include "DropBoxConnectController.hpp"
#include "Image/HFRNetworkAccessManager.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>


DropBoxConnectController::DropBoxConnectController(QObject *parent) : QObject(parent), m_WebView(NULL), m_Settings(NULL) {

    m_Settings = new QSettings("Amonchakai", "Hg10");
}


void DropBoxConnectController::logInRequest() {
    if(m_WebView == NULL)
        return;

    QFile dropboxInfo(QDir::currentPath() + "/app/native/assets/dropbox_secret.txt");
     if(!dropboxInfo.open(QIODevice::ReadOnly))
         return;

     QTextStream stream(&dropboxInfo);
     QString ClientId = stream.readLine();
     QString ClientSecret = stream.readLine();

     dropboxInfo.close();

     m_Settings->setValue("DropboxClient_ID", ClientId);
     m_Settings->setValue("DropboxClientSecret", ClientSecret);

     m_WebView->setUrl(QString("https://www.dropbox.com/1/oauth2/authorize?")
                             + "&response_type=code"
                             + "&client_id=" + ClientId);
}

void DropBoxConnectController::setWebView(QObject *webView) {
    m_WebView = dynamic_cast<bb::cascades::WebView*>(webView);
}

void DropBoxConnectController::getToken(const QString &code) {
    QNetworkRequest request(QUrl("https://api.dropbox.com/1/oauth2/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl params;
    params.addQueryItem("code",         code);
    params.addQueryItem("client_id",    m_Settings->value("DropboxClient_ID").value<QString>());
    params.addQueryItem("client_secret", m_Settings->value("DropboxClientSecret").value<QString>());
    params.addQueryItem("grant_type", "authorization_code");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}




void DropBoxConnectController::checkReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                parse(response);
            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }

}


void DropBoxConnectController::parse(const QString &message) {
    //qDebug() << message.mid(0, 3);
    //qDebug() << message.mid(5);

    QRegExp access_token("\"access.token\"[: ]+\"([^\"]+)\"");
    access_token.setMinimal(true);

    QRegExp uid("\"uid\"[: ]+([0-9]+),");
    uid.setMinimal(true);


    if(access_token.indexIn(message) != -1)
        m_Settings->setValue("dropbox_access_token", access_token.cap(1));

    if(uid.indexIn(message) != -1)
         m_Settings->setValue("dropbox_uid", uid.cap(1));

    if(!access_token.cap(1).isEmpty())
        emit closeConnect();

}


void DropBoxConnectController::putFile(const QString &path) {

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QString name = path.mid(path.lastIndexOf("/")+1);

    QUrl url("https://api-content.dropbox.com/1/files_put/auto/" + name + "?access_token=" + m_Settings->value("dropbox_access_token").value<QString>() + "&autorename=true");

    QHttpPart filePart;
    QNetworkRequest request(url);

    m_LastUploadedFile = "";

    QNetworkReply* reply = HFRNetworkAccessManager::get()->put(request, file.readAll());
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkUploadReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    ok = connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploading(qint64, qint64)));
    Q_ASSERT(ok);
}


void DropBoxConnectController::checkUploadReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QRegExp path("path\": \"/([^\"]+)\"");
                if(path.indexIn(response) != -1) {
                    m_LastUploadedFile = path.cap(1);
                }

                emit uploaded();
            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}

void DropBoxConnectController::uploading(qint64 status, qint64 total) {
    emit uploading(100 * status / total);
}



void DropBoxConnectController::share() {
    QNetworkRequest request(QUrl("https://api.dropbox.com/1/shares/auto/" + m_LastUploadedFile + "?access_token=" + m_Settings->value("dropbox_access_token").value<QString>()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl params;
    params.addQueryItem("short_url",    "true");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkShareReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}


void DropBoxConnectController::checkShareReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                qDebug() << response;

                QRegExp path("url\": \"([^\"]+)\"");
                if(path.indexIn(response) != -1) {
                    m_ShareUrl = path.cap(1);
                    emit shared(m_ShareUrl);
                }
            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}

