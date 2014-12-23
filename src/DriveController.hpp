/*
 * DriveController.hpp
 *
 *  Created on: 22 déc. 2014
 *      Author: pierre
 */

#ifndef DRIVECONTROLLER_HPP_
#define DRIVECONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <QList>
#include <QReadWriteLock>
#include <bb/system/SystemUiResult>

class GoogleConnectController;
class DriveItem;

class DriveController : public QObject {
    Q_OBJECT;


private:
    bb::cascades::ListView          *m_ListView;
    GoogleConnectController         *m_Google;
    QList<DriveItem *>               m_DriveItems;
    QReadWriteLock                   m_Mutex;
    QString                          m_SelectedItemForSharing;




public:
    DriveController            (QObject *parent = 0);
    virtual ~DriveController   ()                      {};




public Q_SLOTS:
    inline void setListView             (QObject *listView)    {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }


    void getFileList                    ();
    void pop                            ();
    void pushDriveItem                  (DriveItem *item);
    void updateView                     ();
    void open                           (const QString &id, const QString &type);
    void onImageReady                   (const QString &url, const QString &diskPath);
    void notifyEmptyFolder              ();
    void createNewFolder                ();
    void setHomeFolder                  (const QString &id);
    void openForSharing                 (const QString &id, const QString &type);
    void copyShareLink                  (const QString& id, const QString &link);

    void onPromptFinishedCreateFolder(bb::system::SystemUiResult::Type);
    void onPromptFinishedShareFile(bb::system::SystemUiResult::Type);



Q_SIGNALS:
    void complete                       ();
    void pushOpenLink                   (const QString &link);


};


#endif /* DRIVECONTROLLER_HPP_ */
