/*
 * HFRNetworkAccessManager.hpp
 *
 *  Created on: 15 mars 2014
 *      Author: PierreL
 */

#ifndef HFRNETWORKACCESSMANAGER_HPP_
#define HFRNETWORKACCESSMANAGER_HPP_

#include <QtCore/QObject>

class QNetworkAccessManager;
class CookieJar;

class HFRNetworkAccessManager : public QObject {
	Q_OBJECT

public:
	// ----------------------------------------------------------------------------------------------
	// member functions

	virtual ~HFRNetworkAccessManager() {};

	static QNetworkAccessManager* get();




private:
	// ----------------------------------------------------------------------------------------------
	// member variables

	static HFRNetworkAccessManager *m_This;
	QNetworkAccessManager *m_NetworkManager;

	// singleton: hide constructor
	HFRNetworkAccessManager(QObject *parent);



	// ----------------------------------------------------------------------------------------------
	// member functions

	inline QNetworkAccessManager* 				getManager()			 {return m_NetworkManager;}

};

#endif /* HFRNETWORKACCESSMANAGER_HPP_ */
