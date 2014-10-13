/*
 * ConversationController.cpp
 *
 *  Created on: 13 oct. 2014
 *      Author: pierre
 */


#include "ConversationController.hpp"
#include "ConversationManager.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>

ConversationController::ConversationController(QObject *parent) : QObject(parent), m_WebView(NULL) {

    bool check = connect(ConversationManager::get(), SIGNAL(historyLoaded()), this, SLOT(updateView()));
    Q_ASSERT(check);
    Q_UNUSED(check);

}

void ConversationController::load(const QString &id) {
    ConversationManager::get()->load(id);
}

void ConversationController::updateView() {
    if(m_WebView == NULL) {
        qWarning() << "did not received the webview. quit.";
        return;
    }

    QFile htmlTemplateFile(QDir::currentPath() + "/app/native/assets/template.html");
    if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
        htmlTemplateFile.setFileName(QDir::currentPath() + "/app/native/assets/template_black.html");
    }
    QFile htmlEndTemplateFile(QDir::currentPath() + "/app/native/assets/template_end.html");

    if (htmlTemplateFile.open(QIODevice::ReadOnly) && htmlEndTemplateFile.open(QIODevice::ReadOnly)) {
        QString htmlTemplate = htmlTemplateFile.readAll();
        QString endTemplate = htmlEndTemplateFile.readAll();


        m_WebView->setHtml(htmlTemplate  + endTemplate, QUrl("local:///assets/"));
    }
}



