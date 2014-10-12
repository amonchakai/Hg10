/*
 * WebResourceManager.h
 *
 *  Created on: 22 mars 2014
 *      Author: PierreL
 */

#ifndef WEBRESOURCEMANAGER_H_
#define WEBRESOURCEMANAGER_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QReadWriteLock>

class WebResourceManager : public QObject {
	Q_OBJECT;

public:
	// ----------------------------------------------------------------------------------------------
	// member functions

	virtual ~WebResourceManager() {};

	static WebResourceManager* get();


	void getImage(const QString& url) const;
	QString getMatchingImage(const QString &url) const;

Q_SIGNALS:
	void onImageReady(const QString &diskPath, const QString &url) const;


public Q_SLOTS:
	void checkReply();
	void cleanup();


private:
	// ----------------------------------------------------------------------------------------------
	// member variables

	static WebResourceManager *m_This;
	QList<QString> 			  *m_DownloadQueue;
	mutable QReadWriteLock	  *m_EditQueue;

	// singleton: hide constructor
	WebResourceManager(QObject *parent);



	// ----------------------------------------------------------------------------------------------
	// member functions

};


#endif /* WEBRESOURCEMANAGER_H_ */
