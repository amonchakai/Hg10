/*
 * CustomizationController.hpp
 *
 *  Created on: 1 févr. 2015
 *      Author: pierre
 */

#ifndef CUSTOMIZATIONCONTROLLER_HPP_
#define CUSTOMIZATIONCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <bb/system/SystemUiResult>

class CustomizationController : public QObject {
    Q_OBJECT;



private:
    bb::cascades::ListView          *m_ListView;



public:
    CustomizationController              (QObject *parent = 0);
    virtual ~CustomizationController     ()                      {};



public Q_SLOTS:
    inline void setListView             (QObject *listView)    {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
    void updateView                     ();
    void deleteCustom                   (const QString &id);

    void createDefault                  ();
    void onPromptFinishedCreateTheme    (bb::system::SystemUiResult::Type);

Q_SIGNALS:


};




#endif /* CUSTOMIZATIONCONTROLLER_HPP_ */
