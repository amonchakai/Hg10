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
#include <bb/system/SystemDialog>
#include <bb/system/SystemListDialog>


ThemeEditorController::ThemeEditorController(QObject *parent) : QObject(parent), m_WebView(NULL), m_TextEditor(NULL), m_listdialog(NULL) {

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
        // preload history

        QString body;
        body +=  QString("<div class=\"bubble-left\"><div class=\"bubble-left-avatar\"><img src=\"images/avatar.png.square.png\" /><div class=\"author-left\"><p>" + tr("Me") +"</p></div><div class=\"time-left\"><p>10:34</p></div></div><br/><br/><br/>")
                                       + "<ul><li><p>Hello</p></li></ul></div><br/>";


        body +=  QString("<div class=\"bubble-right\"><div class=\"bubble-right-avatar\"><img src=\"images/avatar.png.square.png\" /><div class=\"author-right\"><p>" + tr("Contact") +"</p></div><div class=\"time-right\"><p>10:36</p></div></div><br/><br/><br/>")
                                               + "<ul><li><p>hi!</p></li></ul></div><br/>";


        m_WebView->setHtml(htmlTemplate + body  + endTemplate, "file:///" + QDir::homePath() + "/../app/native/assets/");
    }
}

void ThemeEditorController::loadEditor() {
    if(!m_TextEditor)
        return;

    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Customization");

    {
        QFile file(directory + "/" +  m_UserId + ".xml");
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QString themeSettings = stream.readAll();

            QRegExp color("<color value=\"([^\"]+)\"");

            if(color.indexIn(themeSettings) != -1) {
                emit colorSet(color.cap(1));
            } else {
                if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
                    emit colorSet("#000000");
                } else {
                    emit colorSet("#c7dfe4");
                }
            }

            file.close();
        }

    }


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


    QString settings;
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        stream << m_TextEditor->text();

        file.close();
    }



    {
        QString backgroundImage;

        QFile file(directory + "/" +  m_UserId + ".xml");
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QString themeSettings = stream.readAll();

            QRegExp wallpaper("<wallpaper url=\"([^\"]+)\"");

            if(wallpaper.indexIn(themeSettings) != -1) {
                backgroundImage = wallpaper.cap(1);
            }


            file.close();

            if (file.open(QIODevice::WriteOnly)) {
                QTextStream stream(&file);

                stream << "<root>";
                stream << "<wallpaper url=\"" + backgroundImage + "\" />";
                stream << "<color value=\"" + m_BackgroundColor + "\" />";
                stream << "</root>";

            }
            file.close();

        }

    }

}





void ThemeEditorController::selectPreset() {
    using namespace bb::cascades;
    using namespace bb::system;

    // Create a SystemListDialog with these characteristics:
    // The "confirmLabel" (OK button) is set to "My favorite"
    // The "cancelLabel" (CANCEL button) is set to "Cancel"
    // This dialog box doesn't have a custom button


    if(m_listdialog == NULL) {
        m_listdialog = new SystemListDialog(tr("OK"), tr("Cancel"));

        m_listdialog->setTitle("Choose a theme");
        m_listdialog->appendItem("BBM");
        m_listdialog->appendItem("Hangouts");
        m_listdialog->appendItem("Simple");

        bool success = connect(m_listdialog,
             SIGNAL(finished(bb::system::SystemUiResult::Type)),
             this,
             SLOT(onDialogThemeFinished(bb::system::SystemUiResult::Type)));

        if (success) {
            // Signal was successfully connected
            // Now show the dialog box in your UI

            m_listdialog->show();
        } else {
            // Failed to connect to signal

            m_listdialog->deleteLater();
        }
    } else {
        m_listdialog->show();
    }

}


void ThemeEditorController::onDialogThemeFinished(bb::system::SystemUiResult::Type result) {

    if(result == bb::system::SystemUiResult::ConfirmButtonSelection) {

        const QList<int> &indices = m_listdialog->selectedIndices();

        if(indices.isEmpty()) {
            return;
        }

        QString filename;
        QString color;
        switch(indices[0]) {
            case 0:
                filename = "bubble";
                color = "#c7dfe4";
                break;

            case 1:
                filename = "bubble_hangouts";
                color = "#e4e4e4";
                break;

            case 2:
                filename = "bubble_simple";
                color = "#e5ded4";
                break;
        }

        if(m_TextEditor == NULL) return;

        QString suffix;
        if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
            suffix = "_black";
        }

        QFile cssTemplateFile;
        cssTemplateFile.setFileName(QDir::currentPath() + "/app/native/assets/" + filename + suffix + ".css");


        if(cssTemplateFile.open(QIODevice::ReadOnly)) {
            m_TextEditor->setText(cssTemplateFile.readAll());
        }

        emit colorSet(color);

        updateView();

    }

}


