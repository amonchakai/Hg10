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

class ThemeEditorController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( QString userId     READ getUserId        WRITE setUserId       NOTIFY  userIdChanged)


private:
    bb::cascades::WebView             *m_WebView;
    bb::cascades::TextArea            *m_TextEditor;
    QString                            m_UserId;


public:
    ThemeEditorController              (QObject *parent = 0);
    virtual ~ThemeEditorController     ()                      {};

    inline const QString &getUserId    ()                       { return m_UserId; }
    inline void setUserId              (const QString &v)       { m_UserId = v; }


public Q_SLOTS:
    inline void setWebView             (QObject *view)    {m_WebView = dynamic_cast<bb::cascades::WebView*>(view);      }
    inline void setEditor              (QObject *view)    {m_TextEditor = dynamic_cast<bb::cascades::TextArea*>(view); loadEditor();  }
    void updateView                    ();
    void loadEditor                    ();
    void saveTheme                     ();

Q_SIGNALS:
    void userIdChanged();

};



#endif /* THEMEEDITORCONTROLLER_HPP_ */
