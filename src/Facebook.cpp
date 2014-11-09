/*
 * Facebook.cpp
 *
 *  Created on: 27 oct. 2014
 *      Author: pierre
 */

#include "Facebook.hpp"
#include "Image/HFRNetworkAccessManager.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>
#include <bb/cascades/ImageTracker>
#include <bb/cascades/WebStorage>
#include <bb/cascades/WebNavigationRequest>
#include <QReadWriteLock>
#include <QTimer>
#include "ConversationManager.hpp"

#include "PrivateAPIKeys.h"
/*
 * #include "PrivateAPIKeys.h"
 *  define:
 *  - FACEBOOK_CIENT_ID
 *
 *  which can be obtained from the Facebook developer console:
 *  https://developers.facebook.com/apps/
 *
 */




QReadWriteLock  facebookMutex;

Facebook::Facebook(QObject *parent) : OnlineHistory(parent), m_WebView(NULL), m_Settings(NULL), m_NbMessages(0) {

    m_Settings = new QSettings("Amonchakai", "Hg10");
}


void Facebook::getAvatar(const QString &id) {

    qDebug() << id << "requested";

    QNetworkRequest request(QUrl(QString("https://graph.facebook.com/")
                                + id
                                + "/picture?type=large&redirect=false"
                           )
                        );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyRedirectUrl()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

}

void Facebook::checkReplyRedirectUrl() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QRegExp url("\"url\"[: ]+\"([^\"]+)\"");
                if(url.indexIn(response) != -1) {
                    QString urlPic = url.cap(1);
                    urlPic.replace("\\/","/");

                    QRegExp idRetriever("https://graph.facebook.com/(.*)/picture");
                    if(idRetriever.indexIn(reply->url().toString()) != -1) {
                        facebookMutex.lockForWrite();
                        m_UrlToUser[urlPic] = idRetriever.cap(1);
                        facebookMutex.unlock();
                    }


                    downloadImage(urlPic);
                }
            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }

}

void Facebook::downloadImage(const QString url_img) {
    QNetworkRequest request(url_img);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkDownload()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}


void Facebook::checkDownload() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                facebookMutex.lockForWrite();
                saveImages(reply->readAll(), m_UrlToUser[reply->url().toString()]);
                facebookMutex.unlock();
            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }

}

void Facebook::saveImages(const QByteArray &buffer, const QString &username) {
    QImage qImage;
    qImage.loadFromData(buffer);

    int minBorder = std::min<int>(qImage.size().height(), qImage.size().width());
    QImage nqImage = qImage.copy((qImage.size().width()-minBorder)/2, 0, minBorder, minBorder);
    qImage = nqImage;

    QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");

    bool isNum;
    qlonglong value = username.toLongLong(&isNum);
    QString prefix("");
    if(isNum) prefix = "-";

    QString name(vCardsDir + "/" + prefix + username + "@chat.facebook.com");

    uchar *bits = qImage.bits();
    int radius = std::min(qImage.size().width(), qImage.size().height())/2; radius = radius*radius;
    int center_x = qImage.size().width() / 2;
    int center_y = qImage.size().height() / 2;
    int depth = qImage.depth() / 8;

    // save two representation of the picture: a square for the post, and a disk for the user list
    if(!buffer.isEmpty()) {
        qImage.save(name + ".png.square.png", "PNG");
    }

    for(int i = 0 ; i < qImage.size().width() ; ++i) {
        for(int j = 0 ; j < qImage.size().height() ; ++j) {
            int dstCenter = (center_x - i)*(center_x - i) + (center_y - j)*(center_y - j);
                if(dstCenter > radius) {
                    for(int c = 0 ; c < depth ; ++c) {
                        bits[(j*qImage.size().width()+i)*depth+c] = 255*(c != 3);
                }
            }
        }
    }

    qImage.convertToFormat(QImage::Format_ARGB4444_Premultiplied).save(name + ".png", "PNG");
    emit imagesRetrieved(username);

}


// --------------------------------------------------------------------------------
// Facebook API

void Facebook::setWebView(QObject *webView) {
    m_WebView = dynamic_cast<bb::cascades::WebView*>(webView);
}


void Facebook::logInRequest() {
    if(m_WebView == NULL)
        return;

    if(m_WebView != NULL) {
        bool check = connect(m_WebView, SIGNAL(navigationRequested(bb::cascades::WebNavigationRequest *)), this, SLOT(navigationRequested(bb::cascades::WebNavigationRequest *)));
        Q_ASSERT(check);
        Q_UNUSED(check);
    }

    m_WebView->storage()->clearCookies();
    m_WebView->setUrl(QUrl(QString("https://www.facebook.com/dialog/oauth?")
                                            + "client_id="+ FACEBOOK_CIENT_ID
                                            + "&redirect_uri=http://localhost/"
                                            + "&response_type=token"
                                            + "&scope=read_mailbox,friends_online_presence,user_about_me,public_profile,user_friends,xmpp_login"
                      ));
}


void Facebook::navigationRequested(bb::cascades::WebNavigationRequest *nav) {

    QRegExp code("http://localhost/.#access_token=([a-zA-Z0-9]+)&expires_in=([0-9]+)");
    if(code.indexIn(nav->url().toString()) != -1) {

        m_Settings->setValue("Facebook_access_token", code.cap(1));
        m_Settings->setValue("Facebook_expires_in", code.cap(2));

        // Stop Google Synch...
        m_Settings->setValue("access_token", "");
        m_Settings->setValue("expires_in",   "");
        m_Settings->setValue("refresh_token", "");

        // get the user ID
        QNetworkRequest request(QUrl(QString("https://graph.facebook.com/me?access_token=") + code.cap(1)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
        bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyUserID()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);


        emit closeConnect();
    }

}

void Facebook::checkReplyUserID() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QRegExp userId("\"id\":\"([0-9]+)\"");
                if(userId.indexIn(response) != -1) {
                    m_Settings->setValue("Facebook_userid", userId.cap(1));
                }

            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}

void Facebook::getMessages (const QString &with, int nbMessages) {

    m_CurrentDst = "";
    m_CurrentDstId = with;
    m_NbMessages = nbMessages;

    QRegExp userId("-([0-9]+)@");
    if(userId.indexIn(with) == -1)
        return;


    // get the name as stored in FB
    QNetworkRequest request(QUrl(QString("https://graph.facebook.com/") + userId.cap(1) + "?access_token=" + m_Settings->value("Facebook_access_token").toString()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyUserName()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

}

void Facebook::checkReplyUserName() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                QRegExp username("\"name\":\"([^\"]+)\",");
                if(username.indexIn(response) != -1) {
                    m_CurrentDst = username.cap(1);
                }

                if(!m_CurrentDst.isEmpty()) {

                    getInbox();

                }

            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}


void Facebook::getInbox() {
    QNetworkRequest request(QUrl(QString("https://graph.facebook.com/me/inbox?access_token=") + m_Settings->value("Facebook_access_token").toString()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReplyInbox()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void Facebook::checkReplyInbox() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());
                response = QString::fromUtf8(buffer);

                parseInbox(response);

            }
        } else {
            qDebug() << "reply... " << reply->errorString();
        }

        reply->deleteLater();
    }
}

void Facebook::parseInbox(const QString &page) {
    m_Messages.clear();
    m_Who.clear();
    m_When.clear();

    QList<int>  commentIdx;
    QRegExp commentRegExp("\"comments\"");

    int pos = 0;
    while((pos = commentRegExp.indexIn(page, pos)) != -1) {
        commentIdx.push_back(pos);

        pos += commentRegExp.matchedLength();
    }
    commentIdx.push_back(page.size());

    int contactP = page.indexOf(m_CurrentDst, commentIdx[0]);
    if(contactP == -1)
        return;

    pos = 0;
    for(int i = 0 ; i < commentIdx.size()-1; ++i) {
        if(contactP > commentIdx.at(i) && contactP < commentIdx.at(i+1)) {
            pos = i;
            break;
        }
    }


    int startIdx = commentIdx[pos];
    int endIdx;
    if(pos+1 < commentIdx.size())
        endIdx = commentIdx[pos+1];
    else
        endIdx = commentIdx[pos];

    QRegExp message("id\":\"([0-9]+)\",\"name\":\"([^\"]+)\".,\"message\":\"([^\"]+)\",\"created_time\":\"([^\"]+)\"");

    pos = startIdx;
    while((pos = message.indexIn(page, pos)) != -1) {
        if(pos > endIdx)
            break;

        qDebug() << message.cap(1) << message.cap(2) << message.cap(4) << message.cap(3);

        QString mess = message.cap(3);
        mess.replace("\\/","/");

        m_Messages.push_back(mess);
        if(message.cap(2) == m_CurrentDst)
            m_Who.push_back(m_CurrentDstId);
        else
            m_Who.push_back(ConversationManager::get()->getUser());
        m_When.push_back(message.cap(4));


        pos += message.matchedLength();
    }


    for(int i = m_Messages.size()-1 ; i > 0 && m_Messages.size() - i-1 < m_NbMessages ; --i) {
        ConversationManager::get()->onlineMessage(m_Who[i], m_Messages[i], m_When[i]);
    }
    m_Pos = m_NbMessages;

}

void Facebook::getRemainingMessages   (QString ) {
    QTimer::singleShot(1000, this, SLOT(sendProgressively()));

}

void Facebook::sendProgressively() {
    ++m_NbMessages;

    if(m_NbMessages == m_Messages.size()) {
        emit synchCompleted();
        return;
    }


    ConversationManager::get()->onlineMessage(m_Who[m_Messages.size() - m_NbMessages-1], m_Messages[m_Messages.size() - m_NbMessages-1], m_When[m_Messages.size() - m_NbMessages-1]);

    QTimer::singleShot(1000, this, SLOT(sendProgressively()));
}
