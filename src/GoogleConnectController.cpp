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


QReadWriteLock  mutexGoogleConnect;

GoogleConnectController::GoogleConnectController(QObject *parent) : OnlineHistory(parent),
        m_WebView(NULL),
        m_Settings(NULL),
        m_HistoryIndex(0),
        m_NBMessageExpected(0),
        m_StopListing(false) {

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

    m_WebView->setUrl(QString("https://accounts.google.com/o/oauth2/auth?")
                            + "scope=https://www.googleapis.com/auth/gmail.readonly https://www.googleapis.com/auth/googletalk https://www.googleapis.com/auth/drive.file"
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
    QRegExp success("Success code=(.*)");
    if(success.indexIn(title) != -1) {
        qDebug() << "Success: " << success.cap(1);
        save(success.cap(1));
        emit closeConnect();
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
        getMessages(m_WithButNoKey, m_NBMessageExpected);
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
                        stream << email.cap(1);

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
    m_WithButNoKey = with;

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


void GoogleConnectController::getMessageList() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    m_MessagesID.clear();
    m_ThreadsID.clear();
    m_Messages.clear();
    m_HistoryID.clear();
    m_Froms.clear();
    m_IdxMessageToPush.clear();

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

    QStringList messagesId;
    QStringList threadsId;
    threadsId.push_back(m_ThreadsID[0]);
    messagesId.push_back(m_MessagesID[0]);
    for(int i = 1 ; i < m_ThreadsID.size() ; ++i) {
        if(m_ThreadsID.at(i) == m_ThreadsID[0]) {
            messagesId.push_back(m_MessagesID.at(i));
            threadsId.push_back(m_ThreadsID.at(i));
        }
    }
    m_ThreadsID  = threadsId;
    m_MessagesID = messagesId;

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

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
             const int available = reply->bytesAvailable();
             if (available > 0) {
                 const QByteArray buffer(reply->readAll());
                 response = QString::fromUtf8(buffer);

                 QRegExp snippet("\"snippet\"[: ]+\"([^\"]+)\"");
                 QRegExp content("\"data\"[: ]+\"([^\"]+)\"");
                 QRegExp histID("\"historyId\"[: ]+\"([0-9]+)\"");
                 QRegExp from("\"value\".+\\u003c(.*)\\u003e\"");
                 from.setMinimal(true);


                 snippet.indexIn(response);
                 int pos = content.indexIn(response);
                 if(pos != -1) {
                     if((pos = histID.indexIn(response, 0)) != -1)
                         if(from.indexIn(response)) {

                             if(snippet.cap(1).length() < 200)
                                 m_Messages.push_back(snippet.cap(1));
                             else
                                 m_Messages.push_back(base64_decode(content.cap(1)));


                             m_Messages.last().replace(andAmp,"&");
                             m_Messages.last().replace(quote,"\"");
                             m_Messages.last().replace(quote2,"\"");
                             m_Messages.last().replace(euro, "e");
                             m_Messages.last().replace(inf, "<");
                             m_Messages.last().replace(sup, ">");


                             m_Froms.push_back(from.cap(1).mid(0, from.cap(1).size()-1));
                             m_HistoryID.push_back(histID.cap(1).toInt());
                             m_IdxMessageToPush.push_front(m_HistoryID.size()-1);

                             checkOrder();

                         }
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
    if(   (m_ThreadsID.size() <= m_HistoryIndex)
       || (m_HistoryIndex >= m_NBMessageExpected)
       || ((m_LastThread != m_ThreadsID[m_HistoryIndex]) && (m_HistoryIndex >= 10))
       || (m_HistoryIndex < m_MessagesID.size() && (m_LastSynchId == m_MessagesID[m_HistoryIndex]))) {

        checkOrder(true);

        ConversationManager::get()->saveHistory();
        return;
    }


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


void GoogleConnectController::checkOrder(bool flush) {
    // -------------
    // bubble sort on timestamp
    bool changedOrder = true;
    while(changedOrder) {
        changedOrder = false;

        for(int n = 1 ; n < m_IdxMessageToPush.size() ; ++n) {
            for(int i = 0 ; i < m_IdxMessageToPush.size()-n ; ++i) {
                if(m_HistoryID[m_IdxMessageToPush[i]] > m_HistoryID[m_IdxMessageToPush[i+1]]) {
                    std::swap(m_IdxMessageToPush[i], m_IdxMessageToPush[i+1]);
                    changedOrder = true;
                }
            }
        }

    }

    if(m_IdxMessageToPush.size() > 5) {
        cleanupMessage(m_Messages[m_IdxMessageToPush.last()]);
        ConversationManager::get()->onlineMessage(m_Froms[m_IdxMessageToPush.last()], m_Messages[m_IdxMessageToPush.last()], m_MessagesID[m_IdxMessageToPush.last()]);
        m_IdxMessageToPush.pop_back();
    }

    if(flush) {
        for(int i = m_IdxMessageToPush.size()-1 ; i >= 0 ; --i) {
            //qDebug() << m_IdxMessageToPush.at(i) << m_Froms.size() << m_Messages.size() << m_MessagesID.size();
            cleanupMessage(m_Messages[m_IdxMessageToPush.at(i)]);
            ConversationManager::get()->onlineMessage(m_Froms[m_IdxMessageToPush.at(i)], m_Messages[m_IdxMessageToPush.at(i)], m_MessagesID[m_IdxMessageToPush.at(i)]);
        }
    }

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

void GoogleConnectController::getRemainingMessages(QString lastMessageId) {
    m_NBMessageExpected = std::numeric_limits<int>::max();
    m_LastSynchId = lastMessageId;

    // remove the last item which was used to detect synch requirements (already in the view)
    if(m_IdxMessageToPush.size() > 0)
        m_IdxMessageToPush.pop_back();

    qDebug() << "[GOOGLECONNECT] entering";


    if(    (m_ThreadsID.size() <= m_HistoryIndex)
        || (m_HistoryIndex < m_MessagesID.size() && (m_LastSynchId == m_MessagesID[m_HistoryIndex]))) {
        //qDebug() << m_ThreadsID.size() << m_HistoryIndex; // << m_LastSynchId << m_MessagesID[m_HistoryIndex];

        checkOrder(true);

        //ConversationManager::get()->saveHistory();
        return;
    }

    qDebug() << "[GOOGLECONNECT] getRemainingMessages";


    // recursive call to get the next messages...
    QNetworkRequest request(QUrl(QString("https://www.googleapis.com/gmail/v1/users/")
                                        + "me"
                                        + "/messages/" + m_MessagesID[m_HistoryIndex]
                                        + "?access_token=" + m_Settings->value("access_token").value<QString>()
                                   )
                                );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(getMessageReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    qDebug() << "[GOOGLECONNECT] leave getRemainingMessages";
}

// -----------------------------------------------------------------------------------------------------------

void GoogleConnectController::putFile(const QString &path) {

}


void GoogleConnectController::checkUploadReply() {

}

void GoogleConnectController::uploading(qint64 status, qint64 total) {

}


