/*
 * ListContactsController.hpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */

#ifndef LISTCONTACTSCONTROLLER_HPP_
#define LISTCONTACTSCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>


class ListContactsController : public QObject {
    Q_OBJECT;


private:
    bb::cascades::ListView   *m_ListView;

public:
    ListContactsController(QObject *parent = 0);



public Q_SLOTS:
    inline void setListView    (QObject *listView)      {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
    void updateView();

    void updateContacts();

Q_SIGNALS:
    void complete();


};



#endif /* LISTCONTACTSCONTROLLER_HPP_ */
