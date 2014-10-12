#include "HFRNetworkAccessManager.hpp"

#include <QNetworkAccessManager>

#include "CookieJar.hpp"

HFRNetworkAccessManager *HFRNetworkAccessManager::m_This = NULL;



HFRNetworkAccessManager::HFRNetworkAccessManager(QObject *parent) : QObject(parent), m_NetworkManager(new QNetworkAccessManager(this)) {

}


// get singleton
QNetworkAccessManager *HFRNetworkAccessManager::get() {
	if(m_This == NULL) {
		// if not already done, instantiate the network manager
		m_This = new HFRNetworkAccessManager(NULL);

		// set cookies jar
		m_This->getManager()->setCookieJar(CookieJar::get());
	}

	return m_This->getManager();
}



