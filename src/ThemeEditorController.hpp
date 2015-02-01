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



private:
    bb::cascades::WebView             *m_WebView;
    bb::cascades::TextArea            *m_TextEditor;



public:
    ThemeEditorController              (QObject *parent = 0);
    virtual ~ThemeEditorController     ()                      {};



public Q_SLOTS:
    inline void setWebView             (QObject *view)    {m_WebView = dynamic_cast<bb::cascades::WebView*>(view);      }
    inline void setEditor              (QObject *view)    {m_TextEditor = dynamic_cast<bb::cascades::TextArea*>(view); loadEditor();  }
    void updateView                    ();
    void loadEditor                    ();

Q_SIGNALS:


};



#endif /* THEMEEDITORCONTROLLER_HPP_ */
