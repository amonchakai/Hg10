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

    Q_PROPERTY( QString audioFile   READ getAudioName)

private:
    bb::cascades::ListView          *m_ListView;
    GoogleConnectController         *m_Google;
    QList<DriveItem *>               m_DriveItems;
    QReadWriteLock                   m_Mutex;
    QString                          m_SelectedItemForSharing;
    QString                          m_AudioName;

    QString                          m_DowloadLocation;


public:
    DriveController            (QObject *parent = 0);
    virtual ~DriveController   ()                      {};




public Q_SLOTS:
    inline void setListView             (QObject *listView)    {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }

    inline const QString &getAudioName  ()                     { return m_AudioName; }

    void getFileList                    ();
    void pop                            ();
    void pushDriveItem                  (DriveItem *item);
    void updateView                     ();
    void open                           (const QString &id, const QString &type);
    void onImageReady                   (const QString &url, const QString &diskPath);
    void notifyEmptyFolder              ();
    void createNewFolder                ();
    void setHomeFolder                  (const QString &id);
    void renameFile                     (const QString &id, const QString &title);
    void openForSharing                 (const QString &id, const QString &type);
    void copyShareLink                  (const QString& id, const QString &link);
    void upload                         (const QString &path);
    void downloadFile                   (const QString &fileUrl, const QString &title);
    void askName                        ();


    void fowardUploading                (int);
    void checkDownload                  ();

    void onPromptFinishedCreateFolder(bb::system::SystemUiResult::Type);
    void onPromptFinishedChooseName(bb::system::SystemUiResult::Type);
    void onPromptFinishedShareFile(bb::system::SystemUiResult::Type);
    void onPromptFinishedRenameFile(bb::system::SystemUiResult::Type);
    void onPromptFinishedDownloadLocation(const QStringList &);
    void onPromptFinishedDownloadLocationCanceled();



Q_SIGNALS:
    void complete                       ();
    void pushOpenLink                   (const QString &link);
    void uploading                      (int status);
    void startRecording                 ();


};


#endif /* DRIVECONTROLLER_HPP_ */
