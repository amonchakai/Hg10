/*
 * NetImageTracker.h
 *
 *  Created on: 22 mars 2014
 *      Author: PierreL
 */

#ifndef NETIMAGETRACKER_H_
#define NETIMAGETRACKER_H_



#include "WebResourceManager.h"
#include <bb/cascades/ImageTracker>

using namespace bb::cascades;

/**
 * The NetImageTracker is used so that Cascades can be informed when an image is downloaded
 * via a NetImageManager.
 */
class NetImageTracker: public bb::cascades::ImageTracker
{
Q_OBJECT

    /**
     * Sets the NetImageTracker source, the remote networked url that points to the image.
     * You need to set one as an attached object in your QML files.
     */
Q_PROPERTY(QString source 		READ source WRITE setSource NOTIFY sourceChanged)
Q_PROPERTY(QString loadingImage READ getLoadingImage WRITE setLoadingImage NOTIFY loadingImageChanged)

public:
    /**
     * Constructor; Sets up the net image view
     * @param parent The parent Container.
     */
    NetImageTracker(QObject *parent = 0);

public slots:

    /**
     * Setter for the source object.
     *
     * @param source in string with path to the source.
     */
    void setSource(const QString &source);
    inline void setLoadingImage(const QString &img)			{m_DefaultImage = img;}
    inline const QString &getLoadingImage()	const			{return m_DefaultImage;}

    /**
     * Getter for source
     * @return a string of the source
     */
    QString source();

signals:
    /**
     * Signal that emits when the source have changed, wont happen automatically
     *
     * @param source the new source
     */
    void sourceChanged(QString source);
    void loadingImageChanged();
    void completed();

private slots:
    /**
     * Emitted when we are done with the setup of this class
     */
    void onCreationCompleted();

    /**
     * Emitted when we have a image that is ready for consumption
     *
     * @param filePath the path to the image that we can do what we want with
     * @param imageName the actual name of the file, useful!
     */
    void onImageReady(const QString filePath, const QString imageName);

private:
    QString mSource;
    QString m_DefaultImage;

    bool mIsCreated;
};

#endif /* NETIMAGETRACKER_H_ */
