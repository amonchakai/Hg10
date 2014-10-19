/*
 * GoogleConnectController.cpp
 *
 *  Created on: 19 oct. 2014
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


GoogleConnectController::GoogleConnectController(QObject *parent) : QObject(parent), m_WebView(NULL), m_Settings(NULL), m_HistoryIndex(0) {
    m_Settings = new QSettings("Amonchakai", "Hg10");
}



void GoogleConnectController::logInRequest() {

    if(m_WebView == NULL)
        return;

    QFile googleInfo(QDir::currentPath() + "/app/native/assets/secret.txt");
    if(!googleInfo.open(QIODevice::ReadOnly))
        return;

    QTextStream stream(&googleInfo);
    QString ClientId = stream.readLine();
    QString ClientSecret = stream.readLine();
    QString APIKey = stream.readLine();

    googleInfo.close();

    m_Settings->setValue("Client_ID", ClientId);
    m_Settings->setValue("ClientSecret", ClientSecret);
    m_Settings->setValue("APIKey", APIKey);

    m_WebView->setUrl(QString("https://accounts.google.com/o/oauth2/auth?")
                            + "scope=https://www.googleapis.com/auth/gmail.readonly"
                            + "&redirect_uri=urn:ietf:wg:oauth:2.0:oob"
                            + "&response_type=code"
                            + "&client_id=" + ClientId);

}

void GoogleConnectController::save(const QString& key) {
    m_Settings->setValue("key", key);
    getToken();

}

void GoogleConnectController::getToken() {

    qDebug() << "Request token...";

    QNetworkRequest request(QUrl("https://accounts.google.com/o/oauth2/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl params;
    params.addQueryItem("code",         m_Settings->value("key").value<QString>());
    params.addQueryItem("client_id",    m_Settings->value("Client_ID").value<QString>());
    params.addQueryItem("client_secret", m_Settings->value("ClientSecret").value<QString>());
    params.addQueryItem("redirect_uri", "urn:ietf:wg:oauth:2.0:oob");
    params.addQueryItem("grant_type", "authorization_code");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}


void GoogleConnectController::renewToken() {
    QNetworkRequest request(QUrl("https://accounts.google.com/o/oauth2/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl params;
    params.addQueryItem("client_id",    m_Settings->value("Client_ID").value<QString>());
    params.addQueryItem("client_secret", m_Settings->value("ClientSecret").value<QString>());
    params.addQueryItem("refresh_token", m_Settings->value("refresh_token").value<QString>());
    params.addQueryItem("grant_type", "refresh_token");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->post(request, params.encodedQuery());
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkRefresh()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}


void GoogleConnectController::checkReply() {
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
    qDebug() << "quit...";
}


void GoogleConnectController::parse(const QString &message) {
    //qDebug() << message.mid(0, 3);
    //qDebug() << message.mid(5);

    QRegExp access_token("\"access.token\"[: ]+\"([^\"]+)\"");
    access_token.setMinimal(true);

    QRegExp expires_in("\"expires.in\"[: ]+([0-9]+),");
    expires_in.setMinimal(true);

    QRegExp refresh_token("\"refresh.token\"[: ]+\"([^\"]+)\"");
    refresh_token.setMinimal(true);

    if(access_token.indexIn(message) != -1)
        m_Settings->setValue("access_token", access_token.cap(1));

    if(expires_in.indexIn(message) != -1)
            m_Settings->setValue("expires_in", expires_in.cap(1));

    if(refresh_token.indexIn(message) != -1)
            m_Settings->setValue("refresh_token", refresh_token.cap(1));

    qDebug() << m_Settings->value("access_token").value<QString>();
    //qDebug() << m_Settings->value("expires_in").value<QString>();
    //qDebug() << m_Settings->value("refresh_token").value<QString>();


}



void GoogleConnectController::checkRefresh() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                parseRefresh(response);
            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
    qDebug() << "quit...";
}


void GoogleConnectController::parseRefresh(const QString &message) {
    //qDebug() << message.mid(0, 3);
    //qDebug() << message.mid(5);

    QRegExp access_token("\"access.token\"[: ]+\"([^\"]+)\"");
    access_token.setMinimal(true);

    QRegExp expires_in("\"expires.in\"[: ]+([0-9]+),");
    expires_in.setMinimal(true);


    if(access_token.indexIn(message) != -1)
        m_Settings->setValue("access_token", access_token.cap(1));

    if(expires_in.indexIn(message) != -1)
        m_Settings->setValue("expires_in", expires_in.cap(1));

}


void GoogleConnectController::getMessages(const QString &with) {
//    https://www.googleapis.com/gmail/v1/users/lebreton.pier%40gmail.com/messages?includeSpamTrash=false&maxResults=2&q=is%3Achat+from%3Amarjot.sylvere&key={YOUR_API_KEY}

    QString user = ConversationManager::get()->getUser().mid(4);
    user.replace("&", "%40");
    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                + "me"
                                + "/messages"
                                + "?access_token=" + m_Settings->value("access_token").value<QString>()
                                + "&q=is:chat from:" + with
                           )
                        );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageList()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}


void GoogleConnectController::getMessageList() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    m_MessagesID.clear();
    m_ThreadsID.clear();

    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
             const int available = reply->bytesAvailable();
             if (available > 0) {
                 const QByteArray buffer(reply->readAll());
                 response = QString::fromUtf8(buffer);

                 QRegExp messageID("\"id\"[: ]+\"([^\"]+)\"");
                 QRegExp threadID("\"threadId\"[: ]+\"([^\"]+)\"");
                 int pos = 0;

                 while((threadID.indexIn(response, pos) != -1) && (pos = messageID.indexIn(response, pos)) != -1) {
                     m_MessagesID.push_back(messageID.cap(1));
                     m_ThreadsID.push_back(threadID.cap(1));

                     pos += messageID.matchedLength();
                 }


             }
        } else {
            qDebug() << "reply... " << reply->errorString();
       }

       reply->deleteLater();
    }


    // -----------------------------------------------------------
    // let's go back in Time...

    m_HistoryIndex = 0;
    if(m_ThreadsID.size() == 0)
        return;

    m_LastThread = m_ThreadsID[0];

    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                    + "me"
                                    + "/messages/" + m_MessagesID[0]
                                    + "?access_token=" + m_Settings->value("access_token").value<QString>()
                               )
                            );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


    reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void GoogleConnectController::getMessageReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
             const int available = reply->bytesAvailable();
             if (available > 0) {
                 const QByteArray buffer(reply->readAll());
                 response = QString::fromUtf8(buffer);

                 QRegExp content("\"snippet\"[: ]+\"([^\"]+)\"");
                 QRegExp histID("\"historyId\"[: ]+\"([0-9]+)\"");

                 int pos = content.indexIn(response);
                 if(pos != -1) {
                     if(histID.indexIn(response, pos) != -1)
                         qDebug() << histID.cap(1) << content.cap(1);
                 }
             }
        } else {
            qDebug() << "reply... " << reply->errorString();
       }

       reply->deleteLater();
    }

    // -----------------------------------------------
    // for now, show only the last thread...

    ++m_HistoryIndex;
    if(m_ThreadsID.size() <= m_HistoryIndex)
        return;

    if(m_LastThread != m_ThreadsID[m_HistoryIndex])
        return;


    // recursive call to get the next messages...
    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                        + "me"
                                        + "/messages/" + m_MessagesID[m_HistoryIndex]
                                        + "?access_token=" + m_Settings->value("access_token").value<QString>()
                                   )
                                );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


    reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}




