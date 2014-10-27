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
#include <QReadWriteLock>

QReadWriteLock  facebookMutex;

Facebook::Facebook(QObject *parent) : QObject(parent) {

}


void Facebook::getAvatar(const QString &id) {

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
                QString name = m_UrlToUser[reply->url().toString()];
                facebookMutex.unlock();
                saveImages(reply->readAll(), name);
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
    QString name(vCardsDir + "/" + username + "@chat.facebook.com");

    uchar *bits = qImage.bits();
    int radius = std::min(qImage.size().width(), qImage.size().height())/2; radius = radius*radius;
    int center_x = qImage.size().width() / 2;
    int center_y = qImage.size().height() / 2;
    int depth = qImage.depth() / 8;

    // save two representation of the picture: a square for the post, and a disk for the user list
    if(!buffer.isEmpty()) {
        qImage.save(name + ".square.png", "PNG");
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
    emit imagesRetrieved();

}


