/*
 * WebResourceManager.cpp
 *
 *  Created on: 22 mars 2014
 *      Author: PierreL
 */

#include "WebResourceManager.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <bb/cascades/ImageTracker>

#include "HFRNetworkAccessManager.hpp"


WebResourceManager *WebResourceManager::m_This = NULL;

WebResourceManager::WebResourceManager(QObject *parent) : QObject(parent), m_DownloadQueue(new QList<QString>()), m_EditQueue(new QReadWriteLock())  {

}


// get singleton
WebResourceManager *WebResourceManager::get() {
	if(m_This == NULL) {
		// if not already done, instantiate the network manager
		m_This = new WebResourceManager(NULL);
	}

	return m_This;
}

void WebResourceManager::cleanup() {
	// check if cache folder exists, if not, creates it
	QString directory = QDir::homePath() + "/Cache/";
	if (!QFile::exists(directory)) {
		return;
	}

	QDateTime now = QDateTime::currentDateTime();;

	QDir dir(directory);
	QStringList fileList = dir.entryList();
	for(int i = 2 ; i < fileList.length() ; ++i) {
		QFileInfo fileInfo(directory + fileList[i]);
		if(fileInfo.lastRead().addDays(3) < now) {
			QFile(directory + fileList[i]).remove();
		}
	}
}

QString WebResourceManager::getMatchingImage(const QString& requestedURL) const {
    // if something explicitly on the disk is requested, then return it
    if(requestedURL.length() > 10 && requestedURL.mid(0, 9).compare("asset:///") == 0) {
        return requestedURL;
    }

    // check if cache folder exists, if not, creates it
    QString directory = QDir::homePath() + "/../native/assets/TMP/";
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }


    // check if image already available
    QUrl url = QUrl(requestedURL);
    // Check if image is stored on disc
    // The qHash is a bucket type hash so the doubling is to remove possible collisions.
    QString diskPath = QDir::homePath() + "/../native/assets/TMP/"
            + QString::number(qHash(url.host())) + "_"
            + QString::number(qHash(url.path())) + ".JPG";

    QFile imageFile(diskPath);

    // If the file exists, send a signal the image is ready
    if (imageFile.exists()) {
        qDebug() << requestedURL << "...existing";
        qDebug() << diskPath;
        return "asset:///TMP/" + QString::number(qHash(url.host())) + "_"
                + QString::number(qHash(url.path())) + ".JPG";
    } else {
        // check if the file is being downloaded, if so, skip it
        m_EditQueue->lockForRead();
        for(int i = 0 ; i < m_DownloadQueue->length() ; ++i)
            if(m_DownloadQueue->at(i).compare(requestedURL) == 0) {
                m_EditQueue->unlock();
                return "asset:///TMP/" + QString::number(qHash(url.host())) + "_"
                + QString::number(qHash(url.path())) + ".JPG";;
            }
        m_EditQueue->unlock();

        // otherwise let's download the file
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


        QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
        bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);

        return "file://" + diskPath;

        qDebug() << requestedURL << "...dowload";
    }

    return "";
}


void WebResourceManager::getImage(const QString& requestedURL) const {

	// if something explicitly on the disk is requested, then return it
	if(requestedURL.length() > 10 && requestedURL.mid(0, 9).compare("asset:///") == 0) {
		emit onImageReady(requestedURL, requestedURL);
		return;
	}

	if(requestedURL.length() > 10 && requestedURL.mid(0, 10).compare("/accounts/") == 0) {
	    emit onImageReady(requestedURL, requestedURL);
	    return;
	}

	// check if cache folder exists, if not, creates it
	QString directory = QDir::homePath() + "/Cache/";
	if (!QFile::exists(directory)) {
		QDir dir;
		dir.mkpath(directory);
	}


	// check if image already available
	QUrl url = QUrl(requestedURL);
	// Check if image is stored on disc
	// The qHash is a bucket type hash so the doubling is to remove possible collisions.
	QString diskPath = QDir::homePath() + "/Cache/"
			+ QString::number(qHash(url.host())) + "_"
			+ QString::number(qHash(url.path())) + ".JPG";

	QFile imageFile(diskPath);

	// If the file exists, send a signal the image is ready
	if (imageFile.exists()) {
		emit onImageReady(requestedURL, diskPath);
	} else {
		// check if the file is being downloaded, if so, skip it
		m_EditQueue->lockForRead();
		for(int i = 0 ; i < m_DownloadQueue->length() ; ++i)
			if(m_DownloadQueue->at(i).compare(requestedURL) == 0) {
				m_EditQueue->unlock();
				return ;
			}
		m_EditQueue->unlock();

		emit onImageReady(requestedURL,"loading");

		// otherwise let's download the file
		QNetworkRequest request(url);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


		QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
		bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
		Q_ASSERT(ok);
		Q_UNUSED(ok);
	}


}

void WebResourceManager::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				QImage qImage;
				qImage.loadFromData(reply->readAll());

				if (qImage.isNull()) {
					return;
				}

				QString diskPath = QDir::homePath() + "/Cache/"
								+ QString::number(qHash(reply->url().host())) + "_"
								+ QString::number(qHash(reply->url().path())) + ".JPG";

				if (qImage.save(diskPath)) {
						emit onImageReady(reply->url().toString(), diskPath);
				}

				// remove item from download queue
				m_EditQueue->lockForWrite();
				for(int i = 0 ; i < m_DownloadQueue->length() ; ++i)
					if(m_DownloadQueue->at(i).compare(reply->url().toString()) == 0) {
						m_DownloadQueue->removeAt(i);
						m_EditQueue->unlock();
						break;
					}
				m_EditQueue->unlock();
			}
		}
	}
}


