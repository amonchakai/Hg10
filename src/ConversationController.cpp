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

    check = connect(ConversationManager::get(), SIGNAL(messageReceived(const QString &, const QString &)), this, SLOT(pushMessage(const QString &, const QString &)));
    Q_ASSERT(check);

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

        const History& history = ConversationManager::get()->getHistory();

        QString body;
        for(int i = 0 ; i < history.m_History.size() ; ++i) {
            const Event &e = history.m_History.at(i);

            if(e.m_Who.toLower() == ConversationManager::get()->getUser().toLower()) {
                body +=  QString("<div class=\"bubble-right\"><img src=\"images/icon.jpg\" />")
                                   + "<p>" + e.m_What + "</p>"
                               + "</div>&nbsp;<br/>";

            } else {
                body +=  QString("<div class=\"bubble-left\"><img src=\"images/icon.jpg\" />")
                                   + "<p>" + e.m_What + "</p>"
                               + "</div>&nbsp;<br/>";
            }
        }


        m_WebView->setHtml(htmlTemplate + body  + endTemplate, QUrl("local:///assets/"));
    }
}


void ConversationController::pushMessage(const QString &from, const QString &message) {
    if(m_WebView == NULL) {
        qWarning() << "did not received the webview. quit.";
        return;
    }

    qDebug() << "push message...";

    bool ownMessage = from.toLower() == ConversationManager::get()->getUser().toLower();

    if(ownMessage)
        m_WebView->evaluateJavaScript("pushMessage(1, \"" + message +"\", \"images/icon.jpg\");");
    else
        m_WebView->evaluateJavaScript("pushMessage(0, \"" + message +"\", \"images/icon.jpg\");");
}


void ConversationController::send(const QString& message) {
    qDebug() << "CALL!";
    ConversationManager::get()->sendMessage(message);

    if(m_WebView == NULL) {
        qWarning() << "did not received the webview. quit.";
        return;
    }
    m_WebView->evaluateJavaScript("pushMessage(1, \"" + message +"\", \"images/icon.jpg\");");
}
