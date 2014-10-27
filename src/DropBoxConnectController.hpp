#ifndef DROPBOXCONNECTCONTROLLER_HPP_
#define DROPBOXCONNECTCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/WebView>
#include <QSettings>


class DropBoxConnectController : public QObject {
    Q_OBJECT;



private:
    bb::cascades::WebView               *m_WebView;
    QSettings                           *m_Settings;

    QString                              m_LastUploadedFile;
    QString                              m_ShareUrl;

public:
    DropBoxConnectController              (QObject *parent = 0);
    virtual ~DropBoxConnectController     ()                      {};

private:
    void parse                  (const QString &message);


public Q_SLOTS:
    void setWebView             (QObject *webView);

    void logInRequest           ();
    void getToken               (const QString &code);
    void checkReply             ();


    void putFile                (const QString &path);
    void checkUploadReply       ();
    void uploading              (qint64 status, qint64 total);

    void share                  ();
    void checkShareReply        ();

Q_SIGNALS:

    void closeConnect           ();
    void uploading              (int status);
    void uploaded               ();
    void shared                 (const QString &url);




};




#endif

