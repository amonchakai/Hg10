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


QReadWriteLock  mutexGoogleConnect;

GoogleConnectController::GoogleConnectController(QObject *parent) : OnlineHistory(parent),
        m_GmailAccess(true),
        m_DriveAccess(1),
        m_WebView(NULL),
        m_Settings(NULL),
        m_HistoryIndex(0),
        m_NBMessageExpected(0),
        m_StopListing(false),
        m_CreatingRootDir(false) {

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
        scope += "https://mail.google.com/ ";

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
            else
                getOnlineTree(data);
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
    uploadFile(path);
}

void GoogleConnectController::uploadFile(const QString &path, const QString &folder) {
    QFile file(path);

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
    if(QDateTime::currentDateTime().date() == date.date()) {
        d->setTimestamp(tr("last edit: ") + date.time().toString("hh:mm:ss"));
    } else {
        d->setTimestamp(tr("last edit: ") + date.date().toString());
    }



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
    d->setTimestamp(date.toString());


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
