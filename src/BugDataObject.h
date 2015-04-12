/*
 * BugDataObject.h
 *
 *  Created on: 5 avr. 2015
 *      Author: pierre
 */

#ifndef BUGDATAOBJECT_H_
#define BUGDATAOBJECT_H_


#include <QtCore/QObject>
#include <QString>


class IssueListItem : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString title       READ getTitle       WRITE setTitle        NOTIFY titleChanged)
    Q_PROPERTY( QString author      READ getAuthor      WRITE setAuthor       NOTIFY authorChanged)
    Q_PROPERTY( QString avatar      READ getAvatar      WRITE setAvatar       NOTIFY avatarChanged)
    Q_PROPERTY( int label           READ getLabel       WRITE setLabel        NOTIFY labelChanged)
    Q_PROPERTY( QString state       READ getState       WRITE setState        NOTIFY stateChanged)
    Q_PROPERTY( bool locked         READ isLocked       WRITE setLocked       NOTIFY lockedChanged)
    Q_PROPERTY( int number          READ getNumber      WRITE setNumber       NOTIFY numberChanged)
    Q_PROPERTY( int comments        READ getComments    WRITE setComments     NOTIFY commentsChanged)

    // ----------------------------------------------------------------------------------------------


private:
    QString m_Title;
    QString m_Author;
    QString m_Avatar;
    int m_Label;
    QString m_State;
    bool m_Locked;
    int m_Number;
    int m_Comments;






    // ----------------------------------------------------------------------------------------------

public:
    IssueListItem(QObject *parent = 0) : QObject(parent), m_Label(0), m_Locked(false) {}
    virtual ~IssueListItem() {}



    inline const QString &getTitle() const                  { return m_Title; }
    inline void           setTitle(const QString &s)        { m_Title = s; }

    inline const QString &getAuthor() const                 { return m_Author; }
    inline void           setAuthor(const QString &c)       { m_Author = c; }

    inline const QString &getAvatar() const                 { return m_Avatar; }
    inline void           setAvatar(const QString &c)       { m_Avatar = c; }

    inline const QString &getState() const                  { return m_State; }
    inline void           setState(const QString &c)        { m_State = c; }

    inline int            getLabel() const                  { return m_Label; }
    inline void           setLabel(int c)                   { m_Label = c; }

    inline bool           isLocked() const                  { return m_Locked; }
    inline void           setLocked(bool c)                 { m_Locked = c; }

    inline int            getNumber() const                 { return m_Number; }
    inline void           setNumber(int c)                  { m_Number = c; }

    inline int            getComments() const               { return m_Comments; }
    inline void           setComments(int c)                { m_Comments = c; }

    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void titleChanged();
        void authorChanged();
        void avatarChanged();
        void labelChanged();
        void stateChanged();
        void lockedChanged();
        void numberChanged();
        void commentsChanged();


};




#endif /* BUGDATAOBJECT_H_ */
