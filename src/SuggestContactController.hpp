/*
 * SuggestContact.hpp
 *
 *  Created on: 1 nov. 2014
 *      Author: pierre
 */

#ifndef SUGGESTCONTACT_HPP_
#define SUGGESTCONTACT_HPP_

#include <bb/cascades/ListView>

class SuggestContactController : public QObject {
    Q_OBJECT;

private:
    bb::cascades::ListView          *m_ListView;


public:
    SuggestContactController              (QObject *parent = 0);
    virtual ~SuggestContactController     ()                               {};



public Q_SLOTS:
    inline void setListView             (QObject *listView)    {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }


    void        suggest                 (const QString &text);
    QString     addParticipant          (const QString &text, const QString &name);



Q_SIGNALS:

};




#endif /* SUGGESTCONTACT_HPP_ */
