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
#include <limits>
#include <QReadWriteLock>
#include <bb/cascades/WebStorage>

#include "DataObjects.h"

#include "PrivateAPIKeys.h"
/*
 * #include "PrivateAPIKeys.h"
 *  define:
 *  - GOOGLE_CIENT_ID
 *  - GOOGLE_CLIENT_SECRET
 *  - GOOGLE_API_KEY
 *
 *  which can be obtained from the google developer console:
 *  https://console.developers.google.com
 *
 */


GoogleConnectController::GoogleConnectController(QObject *parent) : OnlineHistory(parent),
        m_GmailAccess(true),
        m_PicasaAccess(true),
        m_DriveAccess(1),
        m_WebView(NULL),
        m_Settings(NULL),
        m_HistoryIndex(0),
        m_NBMessageExpected(0),
        m_KeepPushing(false),
        m_StopListing(false),
        m_CreatingRootDir(false),
        m_LastSynch(0) {

    m_Settings = new QSettings("Amonchakai", "Hg10");
}

bool GoogleConnectController::isLogged() {
    return !m_Settings->value("access_token").value<QString>().isEmpty();
}

void GoogleConnectController::logInRequest() {

    if(m_WebView == NULL)
        return;


    m_Settings->setValue("Client_ID",    GOOGLE_CIENT_ID);
    m_Settings->setValue("ClientSecret", GOOGLE_CLIENT_SECRET);
    m_Settings->setValue("APIKey",       GOOGLE_API_KEY);

    QString scope = "https://www.googleapis.com/auth/googletalk ";
    if(m_GmailAccess)
        scope += "https://www.googleapis.com/auth/gmail.readonly ";
    else
        scope += "https://mail.google.com/ "; // just to be able to get user email address which is required for oauth authentification

    switch(m_DriveAccess) {
        case 1:
            scope += "https://www.googleapis.com/auth/drive";
            break;
        case 2:
            scope += "https://www.googleapis.com/auth/drive.file";
            break;
        default:
            break;
    }

    if(m_PicasaAccess)
        scope += " https://picasaweb.google.com/data ";

    m_WebView->setUrl(QString("https://accounts.google.com/o/oauth2/auth?")
                            + "scope=" + scope
                            + "&redirect_uri=urn:ietf:wg:oauth:2.0:oob"
                            + "&response_type=code"
                            + "&client_id=" + GOOGLE_CIENT_ID);

}

void GoogleConnectController::setWebView(QObject *webView) {
    m_WebView = dynamic_cast<bb::cascades::WebView*>(webView);

    if(m_WebView != NULL) {
        bool check = connect(m_WebView, SIGNAL(titleChanged(const QString &)), this, SLOT(webviewTitleChanged(const QString& )));
        Q_ASSERT(check);
        Q_UNUSED(check);
    }
}

void GoogleConnectController::webviewTitleChanged(const QString &title) {

    qDebug() << "Success code: " << title;

    QRegExp success("Success code=(.*)");
    if(success.indexIn(title) != -1) {
        save(success.cap(1));
        emit closeConnect();
    } else {
        QRegExp success("code=(.*)");
        if(success.indexIn(title) != -1) {
            save(success.cap(1));
            emit closeConnect();
        }
    }

    if(title == "Denied error=access_denied") {
        qDebug() << "Denied!";
        emit closeConnect();
    }
}


void GoogleConnectController::save(const QString& key) {
    m_Settings->setValue("key", key);
    getToken();

}

void GoogleConnectController::getToken() {

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

    //qDebug() << m_Settings->value("access_token").value<QString>();
    //qDebug() << m_Settings->value("expires_in").value<QString>();
    //qDebug() << m_Settings->value("refresh_token").value<QString>();


    // Stop facebook synch
    m_Settings->setValue("Facebook_access_token", "");
    m_Settings->setValue("Facebook_expires_in", "");

    if(ConversationManager::get()->getUser().isEmpty()) {
        qDebug() << "getting user data";
        getUserInfo();
    } else {
        ConversationManager::get()->setUser(ConversationManager::get()->getUser());
        emit contactInfoObtained();
    }
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


    // Stop facebook synch
    m_Settings->setValue("Facebook_access_token", "");
    m_Settings->setValue("Facebook_expires_in", "");

    if(access_token.indexIn(message) != -1) {
        int pos = m_WithButNoKey.indexOf(":");
        if(pos == -1)
            return;

        QString query = m_WithButNoKey.mid(0,pos);
        QString data = m_WithButNoKey.mid(pos+1);

        qDebug() << "token renewed: " << query << data;

        if(query == "HISTORY_REQUEST")
            getMessages(data, m_NBMessageExpected);
        else {
            if(query == "DIRECTORY_REQUEST")
                getFileList(data);
            else {
                if(query == "MESSAGE_LIST_REQUEST")
                    getRawMessageList(data);
                else
                    getOnlineTree(data);
            }
        }
    }
}

void GoogleConnectController::getUserInfo() {
    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                    + "me"
                                    + "/profile"
                                    + "?access_token=" + m_Settings->value("access_token").value<QString>()
                               )
                            );

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        mutexGoogleConnect.lockForWrite();
        m_StopListing = true;
        mutexGoogleConnect.unlock();

        QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
        bool ok = connect(reply, SIGNAL(finished()), this, SLOT(replyGetUserInfo()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
}

void GoogleConnectController::replyGetUserInfo() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QRegExp email("\"emailAddress\"[: ]+\"([^\"]+)\"");
                if(email.indexIn(response) != -1) {

                    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
                    if (!QFile::exists(directory)) {
                        QDir dir;
                        dir.mkpath(directory);
                    }

                    QFile file(directory + "/UserID.txt");

                    if (file.open(QIODevice::WriteOnly)) {
                        QDataStream stream(&file);
                        stream << email.cap(1).toLower();

                        qDebug() << "USER DATA OBTAINED: " << email.cap(1);

                        file.close();
                        m_Settings->setValue("User", email.cap(1));
                        ConversationManager::get()->setUser(email.cap(1));


                        emit contactInfoObtained();
                    }
                }

            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}


void GoogleConnectController::getMessages(const QString &with, int nbMessages) {


    m_NBMessageExpected = nbMessages;
    m_WithButNoKey = "HISTORY_REQUEST:" + with;

    QString user = ConversationManager::get()->getUser().mid(4);
    user.replace("&", "%40");
    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                + "me"
                                + "/messages"
                                + "?access_token=" + m_Settings->value("access_token").value<QString>()
                                + "&q=is:chat " + with
                           )
                        );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    mutexGoogleConnect.lockForWrite();
    m_StopListing = true;
    mutexGoogleConnect.unlock();

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageList()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}


void GoogleConnectController::getRawMessageList(const QString& with) {
    m_WithButNoKey = "MESSAGE_LIST_REQUEST:" + with;

    QString user = ConversationManager::get()->getUser().mid(4);
    user.replace("&", "%40");
    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                + "me"
                                + "/threads"
                                + "?access_token=" + m_Settings->value("access_token").value<QString>()
                                + "&q=is:chat " + with
                           )
                        );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    mutexGoogleConnect.lockForWrite();
    m_StopListing = true;
    mutexGoogleConnect.unlock();

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(rawMessageListReceived()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void GoogleConnectController::rawMessageListReceived () {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
             const int available = reply->bytesAvailable();
             if (available > 0) {
                 emit rawMessageList(reply->readAll());
             }
        } else {
            qDebug() << "reply... " << reply->errorString();
            qDebug() << "try to renew the key";
            renewToken();
       }

       reply->deleteLater();
    }
}

void GoogleConnectController::getRawMessage(const QString& id) {

    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                + "me"
                                + "/threads/" + id
                                + "?access_token=" + m_Settings->value("access_token").value<QString>()
                           )
                        );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    mutexGoogleConnect.lockForWrite();
    m_StopListing = true;
    mutexGoogleConnect.unlock();

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(rawMessageReceived()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void GoogleConnectController::rawMessageReceived() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
             const int available = reply->bytesAvailable();
             if (available > 0) {
                 emit rawMessage(reply->readAll());
             }
        } else {
            qDebug() << "reply... " << reply->errorString();
       }

       reply->deleteLater();
    }
}

void GoogleConnectController::getMessageList() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if(!m_KeepPushing) {
        m_MessagesID.clear();
        m_ThreadsID.clear();
        m_Messages.clear();
        m_HistoryID.clear();
        m_Froms.clear();
    }

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
            qDebug() << "try to renew the key";
            renewToken();
       }

       reply->deleteLater();
    }

    // last process need to end, before starting a new one.

    mutexGoogleConnect.lockForWrite();
    m_StopListing = false;
    mutexGoogleConnect.unlock();

    // -----------------------------------------------------------
    // let's go back in Time...

    m_HistoryIndex = 0;
    if(m_ThreadsID.size() == 0)
        return;


    m_LastThread = m_ThreadsID[0];

    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                    + "me"
                                    + "/threads/" + m_ThreadsID[0]
                                    + "?access_token=" + m_Settings->value("access_token").value<QString>()
                               )
                            );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

QString base64_decode(const QString &string){
    QString clean(string);
    clean.replace('-', '+');
    clean.replace('_', '/');

    QByteArray ba;
    ba.append(clean);

    return QTextCodec::codecForName("UTF-8")->toUnicode(QByteArray::fromBase64(ba));
}

void GoogleConnectController::getMessageReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    mutexGoogleConnect.lockForRead();
    if(m_StopListing) {
        mutexGoogleConnect.unlock();
        reply->deleteLater();
        return;
    }
    mutexGoogleConnect.unlock();

    QRegExp andAmp("&amp;");
    QRegExp quote("&#034;");
    QRegExp euro("&euro;");
    QRegExp inf("&lt;");
    QRegExp sup("&gt;");
    QRegExp quote2("&quot;");

    QList<QString> local_messages, local_froms, local_histId;

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
             const int available = reply->bytesAvailable();
             if (available > 0) {


                 const QByteArray buffer(reply->readAll());
                 response = QString::fromUtf8(buffer);

                 QRegExp snippet("\"snippet\"[: ]+\"([^\"]*)\"");
                 QRegExp content("\"data\"[: ]+\"([^\"]+)\"");
                 QRegExp histID("\"historyId\"[: ]+\"([0-9]+)\"");
                 QRegExp from("\"value\".+\\u003c(.*)\\u003e\"");
                 from.setMinimal(true);

                 int pos = 0;

                 while(pos != -1) {
                     pos = snippet.indexIn(response, pos);
                     if(pos != -1) {
                         pos += snippet.matchedLength();
                         pos = histID.indexIn(response, pos);
                         pos += histID.matchedLength();

                         pos =from.indexIn(response, pos);
                         pos += from.matchedLength();

                         pos =content.indexIn(response, pos);
                         pos += content.matchedLength();

                         if(snippet.cap(1).length() < 200)
                             local_messages.push_back(snippet.cap(1));
                          else
                              local_messages.push_back(base64_decode(content.cap(1)));

                         local_messages.last().replace(andAmp,"&");
                         local_messages.last().replace(quote,"\"");
                         local_messages.last().replace(quote2,"\"");
                         local_messages.last().replace(euro, "e");
                         local_messages.last().replace(inf, "<");
                         local_messages.last().replace(sup, ">");

                         local_histId.push_back(histID.cap(1));
                         local_froms.push_back(from.cap(1).mid(0, from.cap(1).size()-1));

                     }
                 }

             }
        } else {
            qDebug() << "reply... " << reply->errorString();
       }

       reply->deleteLater();
    }

    // --------------------------------------------
    // check if same as history

    {
        const History &hist = ConversationManager::get()->getHistory();
        if(!hist.m_History.isEmpty() && !local_messages.isEmpty()) {
            if(hist.m_History.last().m_What == local_messages.last())
                return;
        }
    }

    // --------------------------------------------
    // count the number of participants in the chat

    {
        bool stop = false;
        QString participant1, participant2;
        for(int i = 0 ; i < local_froms.size() && !stop ; ++i) {
            if(participant1.isEmpty()) {
                participant1 = local_froms[i];
                continue;
            } else {
                if(participant1 != local_froms[i]) {
                    if(participant2.isEmpty()) {
                        participant2 = local_froms[i];
                    } else {
                        if(participant2 != local_froms[i]) {
                            stop = true;
                            qDebug() << "At least 3 participants: " << participant1 << participant2 << local_froms[i];
                        }
                    }
                }
            }
        }

        // if the two participants are not the current user, then there was 3...
        //if(participant1 != ConversationManager::get()->getUser() && participant2 != ConversationManager::get()->getUser())
        //    stop = true;

        if(stop) {
            qDebug() << "Check another thread";

            m_HistoryIndex++;
            while(m_ThreadsID.size() > m_HistoryIndex && m_ThreadsID[m_HistoryIndex] == m_ThreadsID[m_HistoryIndex-1]) m_HistoryIndex++;

            if(m_ThreadsID.size() <= m_HistoryIndex) return;

            QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                            + "me"
                                            + "/threads/" + m_ThreadsID[m_HistoryIndex]
                                            + "?access_token=" + m_Settings->value("access_token").value<QString>()
                                       )
                                    );

            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

            reply = HFRNetworkAccessManager::get()->get(request);
            bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageReply()));
            Q_ASSERT(ok);
            Q_UNUSED(ok);

            return;
        }
    }



    if(m_KeepPushing) {
        local_messages.append(m_Messages);
        local_histId.append(m_MessagesID);
        local_froms.append(m_Froms);

        m_Messages = local_messages;
        m_MessagesID = local_histId;
        m_Froms = local_froms;
    } else {
        m_Messages = local_messages;
        m_MessagesID = local_histId;
        m_Froms = local_froms;
    }

    if(m_Messages.size() < m_NBMessageExpected) {
        m_KeepPushing = true;

        m_HistoryIndex++;
        while(m_ThreadsID.size() > m_HistoryIndex && m_ThreadsID[m_HistoryIndex] == m_ThreadsID[m_HistoryIndex-1]) m_HistoryIndex++;

        if(m_ThreadsID.size() > m_HistoryIndex) {

            QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                            + "me"
                                            + "/threads/" + m_ThreadsID[m_HistoryIndex]
                                            + "?access_token=" + m_Settings->value("access_token").value<QString>()
                                       )
                                    );

            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

            reply = HFRNetworkAccessManager::get()->get(request);
            bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageReply()));
            Q_ASSERT(ok);
            Q_UNUSED(ok);

            return;
        }

    } else {
        m_KeepPushing = false;
    }


    ConversationManager::get()->flushHistory();
    for(int i = m_Messages.size()-1 ; i >= 0  ; --i) {
        cleanupMessage(m_Messages[i]);
//        qDebug() << ">> " << m_Froms[i] << m_Messages[i];
        ConversationManager::get()->onlineMessage(m_Froms[i], m_Messages[i], m_MessagesID[i]);
    }

    ConversationManager::get()->saveHistory();

}

void GoogleConnectController::getMoreMessages() {
    m_KeepPushing = true;

    m_HistoryIndex++;
    while(m_ThreadsID.size() > m_HistoryIndex && m_ThreadsID[m_HistoryIndex] == m_ThreadsID[m_HistoryIndex-1]) m_HistoryIndex++;

    if(m_ThreadsID.size() > m_HistoryIndex) {

        QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                        + "me"
                                        + "/threads/" + m_ThreadsID[m_HistoryIndex]
                                        + "?access_token=" + m_Settings->value("access_token").value<QString>()
                                   )
                                );

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
        bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageReplyAdd()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);

    }
}



void GoogleConnectController::getMessageReplyAdd() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    mutexGoogleConnect.lockForRead();
    if(m_StopListing) {
        mutexGoogleConnect.unlock();
        reply->deleteLater();
        return;
    }
    mutexGoogleConnect.unlock();

    QRegExp andAmp("&amp;");
    QRegExp quote("&#034;");
    QRegExp euro("&euro;");
    QRegExp inf("&lt;");
    QRegExp sup("&gt;");
    QRegExp quote2("&quot;");

    QList<QString> local_messages, local_froms, local_histId;

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
             const int available = reply->bytesAvailable();
             if (available > 0) {


                 const QByteArray buffer(reply->readAll());
                 response = QString::fromUtf8(buffer);

                 QRegExp snippet("\"snippet\"[: ]+\"([^\"]*)\"");
                 QRegExp content("\"data\"[: ]+\"([^\"]+)\"");
                 QRegExp histID("\"historyId\"[: ]+\"([0-9]+)\"");
                 QRegExp from("\"value\".+\\u003c(.*)\\u003e\"");
                 from.setMinimal(true);

                 int pos = 0;

                 while(pos != -1) {
                     pos = snippet.indexIn(response, pos);
                     if(pos != -1) {
                         pos += snippet.matchedLength();
                         pos = histID.indexIn(response, pos);
                         pos += histID.matchedLength();

                         pos =from.indexIn(response, pos);
                         pos += from.matchedLength();

                         pos =content.indexIn(response, pos);
                         pos += content.matchedLength();

                         if(snippet.cap(1).length() < 200)
                             local_messages.push_back(snippet.cap(1));
                          else
                              local_messages.push_back(base64_decode(content.cap(1)));

                         local_messages.last().replace(andAmp,"&");
                         local_messages.last().replace(quote,"\"");
                         local_messages.last().replace(quote2,"\"");
                         local_messages.last().replace(euro, "e");
                         local_messages.last().replace(inf, "<");
                         local_messages.last().replace(sup, ">");

                         local_histId.push_back(histID.cap(1));
                         local_froms.push_back(from.cap(1).mid(0, from.cap(1).size()-1));

                     }
                 }

             }
        } else {
            qDebug() << "reply... " << reply->errorString();
       }

       reply->deleteLater();
    }


    // --------------------------------------------
    // count the number of participants in the chat

    {
        bool stop = false;
        QString participant1, participant2;
        for(int i = 0 ; i < local_froms.size() && !stop ; ++i) {
            if(participant1.isEmpty()) {
                participant1 = local_froms[i];
                continue;
            } else {
                if(participant1 != local_froms[i]) {
                    if(participant2.isEmpty()) {
                        participant2 = local_froms[i];
                    } else {
                        if(participant2 != local_froms[i]) {
                            stop = true;
                            qDebug() << "At least 3 participants: " << participant1 << participant2 << local_froms[i];
                        }
                    }
                }
            }
        }

        // if the two participants are not the current user, then there was 3...
        if(participant1 != ConversationManager::get()->getUser() && participant2 != ConversationManager::get()->getUser())
            stop = true;

        if(stop) {
            qDebug() << "Check another thread";

            m_HistoryIndex++;
            while(m_ThreadsID.size() > m_HistoryIndex && m_ThreadsID[m_HistoryIndex] == m_ThreadsID[m_HistoryIndex-1]) m_HistoryIndex++;

            if(m_ThreadsID.size() <= m_HistoryIndex) return;

            QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                            + "me"
                                            + "/threads/" + m_ThreadsID[m_HistoryIndex]
                                            + "?access_token=" + m_Settings->value("access_token").value<QString>()
                                       )
                                    );

            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

            reply = HFRNetworkAccessManager::get()->get(request);
            bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageReplyAdd()));
            Q_ASSERT(ok);
            Q_UNUSED(ok);

            return;
        }
    }

    for(int i = local_messages.size()-1 ; i >= 0  ; --i) {
        cleanupMessage(local_messages[i]);
        ConversationManager::get()->onlineMessage(local_froms[i], local_messages[i], local_histId[i]);
    }

    ConversationManager::get()->saveHistory();

}


// remove some HTML extra-stuff. <a></a>
void GoogleConnectController::cleanupMessage(QString& message) {
    QRegExp url("<a href=\"([^\"]+)\">[^<]+</a>");
    int lastPos = 0;
    int pos = 0;
    QString cleanMessage;
    while((pos = url.indexIn(message, lastPos)) != -1) {
        if(pos-lastPos != 0)
            cleanMessage += message.mid(lastPos, pos-lastPos) + " " + url.cap(1);
        else
            cleanMessage += url.cap(1);

        lastPos = pos + url.matchedLength();
    }
    cleanMessage += message.mid(lastPos);

    message = cleanMessage;

}


