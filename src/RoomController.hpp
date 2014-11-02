/*
 * RoomController.hpp
 *
 *  Created on: 1 nov. 2014
 *      Author: pierre
 */

#ifndef ROOMCONTROLLER_HPP_
#define ROOMCONTROLLER_HPP_

#include <bb/cascades/ListView>

class RoomController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( QString avatar      READ getAvatar      WRITE setAvatar      NOTIFY avatarChanged)


private:
    bb::cascades::ListView          *m_ListView;
    QString                          m_Avatar;



public:
    RoomController              (QObject *parent = 0);
    virtual ~RoomController     ()                               {};


    inline const QString &getAvatar     () const               { return m_Avatar; }
    inline void           setAvatar     (const QString &c)     { m_Avatar = c; emit avatarChanged(); }



public Q_SLOTS:
    inline void setListView             (QObject *listView)    {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }

    void createRoom                     (const QString &participants);

    void deleteRoom                     (const QString &roomId);

    // rendering functions
    void pushRoom                       (const QString &room);
    void updateView                     ();


Q_SIGNALS:
    void avatarChanged                  ();

};



#endif /* ROOMCONTROLLER_HPP_ */
