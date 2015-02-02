/*
 * ThemeEditorController.cpp
 *
 *  Created on: 1 févr. 2015
 *      Author: pierre
 */

#include "ThemeEditorController.hpp"
#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>
#include <bb/cascades/WebView>

ThemeEditorController::ThemeEditorController(QObject *parent) : QObject(parent), m_WebView(NULL), m_TextEditor(NULL) {

}



void ThemeEditorController::updateView() {
    if(m_WebView == NULL) {
        qWarning() << "did not received the webview. quit.";
        return;
    }

    QFile htmlTemplateFile(QDir::currentPath() + "/app/native/assets/template.html");
    if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
        htmlTemplateFile.setFileName(QDir::currentPath() + "/app/native/assets/template_black.html");
    }
    QFile htmlEndTemplateFile(QDir::currentPath() + "/app/native/assets/template_end.html");


    // -----------------------------------------------------------------------------------------------
    // customize template
    if (htmlTemplateFile.open(QIODevice::ReadOnly) && htmlEndTemplateFile.open(QIODevice::ReadOnly)) {
        QString htmlTemplate = htmlTemplateFile.readAll();
        QString endTemplate = htmlEndTemplateFile.readAll();

        if(m_TextEditor) {
            QString suffix;
            if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
                suffix = "_black";
            }

            htmlTemplate.replace("</style><link rel=\"stylesheet\" href=\"bubble" + suffix + ".css\">", m_TextEditor->text() + "\n\r</style>");
        }

       // -----------------------------------------------------------------------------------------------
       // choose background image
       /* {
            QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Customization");
            QString filename;
            if(QFile::exists(directory + "/" + ConversationManager::get()->getAdressee() + ".xml")) {
                filename = directory + "/" + ConversationManager::get()->getAdressee() + ".xml";
            } else {
                if(QFile::exists(directory +"/default.xml")) {
                    filename = directory + "/default.xml";
                }
            }

            if(!filename.isEmpty()) {
                QFile file(filename);
                if (file.open(QIODevice::ReadOnly)) {
                    QTextStream stream(&file);
                    QString themeSettings = stream.readAll();

                    QRegExp wallpaper("<wallpaper url=\"([^\"]+)\"");
                    if(wallpaper.indexIn(themeSettings) != -1) {
                        emit wallpaperChanged("file://" + wallpaper.cap(1));

                    }

                    file.close();
                }
            }
        }*/

        // -----------------------------------------------------------------------------------------------
        // preload history

        QString body;
        body +=  QString("<div class=\"bubble-right\"><div class=\"bubble-right-avatar\"><img src=\"images/avatar.png.square.png\" /></div>")
                           + "<p>Hello</p>"
                       + "</div>";

        body +=  QString("<div class=\"bubble-left\"><div class=\"bubble-left-avatar\"><img src=\"images/avatar.png.square.png\" /></div>")
                           + "<p>Hello</p>"
                       + "</div>";


        m_WebView->setHtml(htmlTemplate + body  + endTemplate, "file:///" + QDir::homePath() + "/../app/native/assets/");
    }
}

void ThemeEditorController::loadEditor() {
    if(!m_TextEditor)
        return;

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Customization");
        QFile file(directory + "/" + m_UserId + ".css" );


    QString suffix;
    if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
        suffix = "_black";
    }

    QFile cssTemplateFile;
    if(QFile::exists(directory + "/" + m_UserId + ".css"))
        cssTemplateFile.setFileName(directory + "/" + m_UserId + ".css");
    else
        cssTemplateFile.setFileName(QDir::currentPath() + "/app/native/assets/bubble" + suffix + ".css");


    if(cssTemplateFile.open(QIODevice::ReadOnly)) {
        m_TextEditor->setText(cssTemplateFile.readAll());
    }
}

void ThemeEditorController::saveTheme() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Customization");
    QFile file(directory + "/" + m_UserId + ".css" );

    qDebug() << directory + "/" + m_UserId + ".css";

    QString settings;
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        stream << m_TextEditor->text();

        file.close();
    }
}

