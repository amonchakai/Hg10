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
	Q_PROPERTY( QString preview     READ getPreview     WRITE setPreview     NOTIFY previewChanged)
	Q_PROPERTY( int     presence    READ getPresence    WRITE setPresence    NOTIFY presenceChanged)
	Q_PROPERTY( int     read        READ getRead        WRITE setRead        NOTIFY readChanged)


	// ----------------------------------------------------------------------------------------------


private:
	QString m_Name;
	QString m_ID;
	QString m_Timestamp;
	QString m_Avatar;
	QString m_Preview;
	int     m_Presence;
	int     m_Read;

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

	inline const QString &getPreview() const                 { return m_Preview; }
	inline void           setPreview(const QString &c)       { m_Preview = c; emit previewChanged(); }

	inline int            getPresence() const                { return m_Presence; }
	inline void           setPresence(int c)                 { m_Presence = c; emit presenceChanged(); }

	inline int            getRead() const                    { return m_Read; }
    inline void           setRead(int c)                     { m_Read = c; emit readChanged(); }

	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void nameChanged();
		void idChanged();
		void timestampChanged();
		void avatarChanged();
		void previewChanged();
		void presenceChanged();
		void readChanged();

};


// ----------------------------------------------------------------------------------------------
// Objects to handle message history

struct TimeEvent {
    QString     m_Who;
    QString     m_What;
    QString     m_When;
    int         m_Read;
    QString     m_MessageID;
};

inline QDataStream &operator<<(QDataStream& stream, const TimeEvent& event) {
    stream << event.m_Who;
    stream << event.m_What;
    stream << event.m_When;
    stream << event.m_Read;
    stream << event.m_MessageID;

    return stream;
}

inline QDataStream &operator>>(QDataStream& stream, TimeEvent& event) {
    stream >> event.m_Who;
    stream >> event.m_What;
    stream >> event.m_When;
    stream >> event.m_Read;
    stream >> event.m_MessageID;

    return stream;
}



struct History {
    QList<TimeEvent>    m_History;
};

inline QDataStream &operator<<(QDataStream& stream, const History& history) {
    for(int i = 0 ; i < history.m_History.size() ; ++i) {
        stream << history.m_History.at(i);
    }

    return stream;
}


inline QDataStream &operator>>(QDataStream& stream, History& history) {

    TimeEvent e;
    do {
        stream >> e;
        if(!e.m_Who.isEmpty())
            history.m_History.push_back(e);
    } while(!e.m_Who.isEmpty());

    return stream;
}




#endif /* DATAOBJECTS_H_ */
