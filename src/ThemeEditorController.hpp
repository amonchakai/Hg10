/*
 * ThemeEditorController.hpp
 *
 *  Created on: 1 févr. 2015
 *      Author: pierre
 */

#ifndef THEMEEDITORCONTROLLER_HPP_
#define THEMEEDITORCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/WebView>
#include <bb/cascades/TextArea>
#include <bb/system/SystemUiResult>

namespace bb
{
    namespace system {
        class SystemListDialog;
    }
}

class ThemeEditorController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( QString userId              READ getUserId              WRITE setUserId             NOTIFY  userIdChanged)
    Q_PROPERTY( QString backgroundColor     READ getBackgroundColor     WRITE setBackgroundColor    NOTIFY  backgroundColorChanged)


private:
    bb::cascades::WebView             *m_WebView;
    bb::cascades::TextArea            *m_TextEditor;
    QString                            m_UserId;
    QString                            m_BackgroundColor;
    bb::system::SystemListDialog      *m_listdialog;

public:
    ThemeEditorController              (QObject *parent = 0);
    virtual ~ThemeEditorController     ()                      {};

    inline const QString &getUserId    ()                       { return m_UserId; }
    inline void setUserId              (const QString &v)       { m_UserId = v; }

    inline const QString &getBackgroundColor()                  { return m_BackgroundColor; }
    inline void setBackgroundColor          (const QString &v)  { m_BackgroundColor = v; }


public Q_SLOTS:
    inline void setWebView             (QObject *view)    {m_WebView = dynamic_cast<bb::cascades::WebView*>(view);      }
    inline void setEditor              (QObject *view)    {m_TextEditor = dynamic_cast<bb::cascades::TextArea*>(view); loadEditor();  }
    void updateView                    ();
    void loadEditor                    ();
    void saveTheme                     ();

    void selectPreset                   ();
    void onDialogThemeFinished          (bb::system::SystemUiResult::Type);

Q_SIGNALS:
    void userIdChanged();
    void backgroundColorChanged();
    void colorSet(const QString &value);

};



#endif /* THEMEEDITORCONTROLLER_HPP_ */
