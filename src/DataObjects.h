/*
 * DataObjects.h
 *
 *  Created on: 16 mars 2014
 *      Author: PierreL
 */

#ifndef DATAOBJECTS_H_
#define DATAOBJECTS_H_

#include <QtCore/QObject>
#include <QString>


class Contact : public QObject {
	Q_OBJECT

	Q_PROPERTY( QString name 		READ getName      	WRITE setName        NOTIFY nameChanged)
	Q_PROPERTY( QString id        	READ getID   	    WRITE setID          NOTIFY idChanged)
	Q_PROPERTY( QString timestamp 	READ getTimestamp 	WRITE setTimestamp   NOTIFY timestampChanged)
	Q_PROPERTY( QString avatar      READ getAvatar      WRITE setAvatar      NOTIFY avatarChanged)

	// ----------------------------------------------------------------------------------------------


private:
	QString m_Name;
	QString m_ID;
	QString m_Timestamp;
	QString m_Avatar;

	// ----------------------------------------------------------------------------------------------

public:
	Contact(QObject *parent = 0) : QObject(parent) {}
	virtual ~Contact() {}


	inline const QString &getName() const					{ return m_Name; }
	inline void			  setName(const QString &s)		    { m_Name = s; emit nameChanged(); }

	inline const QString &getID() const				        { return m_ID; }
	inline void			  setID(const QString &c)		    { m_ID = c; emit idChanged();}


	inline const QString &getTimestamp() const				{ return m_Timestamp; }
	inline void			  setTimestamp(const QString &c)	{ m_Timestamp = c; emit timestampChanged(); }

	inline const QString &getAvatar() const                 { return m_Avatar; }
	inline void           setAvatar(const QString &c)       { m_Avatar = c; emit avatarChanged(); }


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void nameChanged();
		void idChanged();
		void timestampChanged();
		void avatarChanged();

};


#endif /* DATAOBJECTS_H_ */
