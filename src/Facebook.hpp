/*
 * Facebook.hpp
 *
 *  Created on: 27 oct. 2014
 *      Author: pierre
 */

#ifndef FACEBOOK_HPP_
#define FACEBOOK_HPP_

#include <QtCore/QObject>
#include <QMap>

class Facebook : public QObject {
    Q_OBJECT;

    QMap<QString, QString>      m_UrlToUser;

public:
    Facebook(QObject *parent);
    virtual ~Facebook()         {};


    void getAvatar      (const QString &user_id);
    void downloadImage  (const QString url);
    void saveImages     (const QByteArray&, const QString &username);

public Q_SLOTS:
    void checkReplyRedirectUrl  ();
    void checkDownload          ();


Q_SIGNALS:
    void imagesRetrieved        (const QString&);

};


#endif /* FACEBOOK_HPP_ */
