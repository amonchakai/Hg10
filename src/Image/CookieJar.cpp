/*
 * CookieJar.cpp
 *
 *  Created on: 15 mars 2014
 *      Author: PierreL
 */

#include "CookieJar.hpp"

#include <QDebug>
#include <QFile>
#include <QDir>

static const unsigned int JAR_VERSION = 23;

CookieJar *CookieJar::m_This = NULL;


CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent) {}

CookieJar *CookieJar::get() {
	if(m_This == NULL)
		m_This = new CookieJar(NULL);

	return m_This;
}

CookieJar::~CookieJar() {

}

/*
 * Code for Loading & Saving from
 * https://qt.gitorious.org/qt/qt/source/6ede9f99f98ed511b78935f7c6537470dce94239:demos/browser/cookiejar.cpp#Lundefined
 */

QDataStream &operator<<(QDataStream &stream, const QList<QNetworkCookie> &list)
{
    stream << JAR_VERSION;
    stream << quint32(list.size());
    for (int i = 0; i < list.size(); ++i)
        stream << list.at(i).toRawForm();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, QList<QNetworkCookie> &list)
{
    list.clear();

    quint32 version;
    stream >> version;

    if (version != JAR_VERSION)
        return stream;

    quint32 count;
    stream >> count;
    for(quint32 i = 0; i < count; ++i)
    {
        QByteArray value;
        stream >> value;
        QList<QNetworkCookie> newCookies = QNetworkCookie::parseCookies(value);
        if (newCookies.count() == 0 && value.length() != 0) {
            qWarning() << "CookieJar: Unable to parse saved cookie:" << value;
        }
        for (int j = 0; j < newCookies.count(); ++j)
            list.append(newCookies.at(j));
        if (stream.atEnd())
            break;
    }
    return stream;
}


void CookieJar::saveToDisk() {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		QDir dir;
		dir.mkpath(directory);
	}

	QFile file(directory + "/SessionCookies.data");

	if (file.open(QIODevice::WriteOnly)) {
		QDataStream stream(&file);
		stream << this->allCookies();

		file.close();
	}

}


void CookieJar::loadFromDisk() {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	QFile file(directory + "/SessionCookies.data");

	if (file.open(QIODevice::ReadOnly)) {
		QDataStream stream(&file);

		QList<QNetworkCookie> cookies;
		stream >> cookies;
		file.close();


		this->setAllCookies(cookies);
	}
}

bool CookieJar::areThereCookies() {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return false;
	}

	QFile file(directory + "/SessionCookies.data");

	if (file.open(QIODevice::ReadOnly)) {
		return true;
	}
	return false;
}


void CookieJar::deleteCookies() {
	QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
	if (!QFile::exists(directory)) {
		return ;
	}

	QFile file(directory + "/SessionCookies.data");
	file.remove();

	this->setAllCookies(QList<QNetworkCookie>());
}

