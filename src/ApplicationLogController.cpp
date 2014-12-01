/*
 * ApplicationLogController.cpp
 *
 *  Created on: 1 déc. 2014
 *      Author: pierre
 */

#include "ApplicationLogController.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>

ApplicationLogController::ApplicationLogController(QObject *parent) : QObject(parent), m_WebView(NULL) {

}


void ApplicationLogController::showLogs() {
    if(m_WebView == NULL) {
        qWarning() << "did not received the webview. quit.";
        return;
    }

    QSettings settings("Amonchakai", "Hg10");

    QFile htmlTemplateFile(QDir::currentPath() + "/app/native/assets/template.html");
    if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
        htmlTemplateFile.setFileName(QDir::currentPath() + "/app/native/assets/template_black.html");
    }
    QFile htmlEndTemplateFile(QDir::currentPath() + "/app/native/assets/template_end.html");


    if (htmlTemplateFile.open(QIODevice::ReadOnly) && htmlEndTemplateFile.open(QIODevice::ReadOnly)) {
        QString htmlTemplate = htmlTemplateFile.readAll();
        QString endTemplate = htmlEndTemplateFile.readAll();

        if(settings.value("fontSize", 28).value<int>() != 28) {
            htmlTemplate.replace("font-size: 28px;", "font-size: " + QString::number(settings.value("fontSize").value<int>()) + "px;");
        }


        QString body;

        QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
        QFile file(directory + "/Logs.txt");
        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }
        QTextStream stream(&file);
        body = stream.readAll();
        file.close();

        m_WebView->setHtml(htmlTemplate + body  + endTemplate, "file:///" + QDir::homePath() + "/../app/native/assets/");
    }

}


void ApplicationLogController::clearLogs() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData");
    QFile file(directory + "/Logs.txt");
    file.remove();

    showLogs();

}
