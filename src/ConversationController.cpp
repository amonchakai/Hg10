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

void ConversationController::load(const QString &id, const QString &avatar) {
    if(avatar.mid(0,9).toLower() == "asset:///")
        m_DstAvatar = QDir::currentPath() + "/app/native/assets/" +  avatar.mid(9);
    else
        m_DstAvatar = avatar;
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

    QString ownAvatar = ConversationManager::get()->getAvatar();
    if(ownAvatar.mid(0,9).toLower() == "asset:///")
        ownAvatar = QDir::currentPath() + "/app/native/assets/" +  ownAvatar.mid(9);


    if (htmlTemplateFile.open(QIODevice::ReadOnly) && htmlEndTemplateFile.open(QIODevice::ReadOnly)) {
        QString htmlTemplate = htmlTemplateFile.readAll();
        QString endTemplate = htmlEndTemplateFile.readAll();

        const History& history = ConversationManager::get()->getHistory();

        QString body;
        for(int i = 0 ; i < history.m_History.size() ; ++i) {
            const TimeEvent &e = history.m_History.at(i);

            if(e.m_Who.toLower() == ConversationManager::get()->getUser().toLower()) {
                body +=  QString("<div class=\"bubble-right\"><img src=\"file:///" + ownAvatar + ".square.png" + "\" />")
                                   + "<p>" + renderMessage(e.m_What) + "</p>"
                               + "</div>";

            } else {
                body +=  QString("<div class=\"bubble-left\"><img src=\"file:///" + m_DstAvatar + ".square.png" + "\" />")
                                   + "<p>" + renderMessage(e.m_What) + "</p>"
                               + "</div>";
            }
        }


        m_WebView->setHtml(htmlTemplate + body  + endTemplate, "file:///" + QDir::homePath() + "/../app/native/assets/");
    }
}

bool ConversationController::isImage(const QString &url) {

    QString ext;
    if(url.length() > 3)
        ext = url.mid(url.size()-3,3);

    if(ext.toLower() == "gif") return true;
    if(ext.toLower() == "png") return true;
    if(ext.toLower() == "jpg") return true;
    if(ext.toLower() == "jpeg") return true;
    if(ext.toLower() == "bmp") return true;
    if(ext.toLower() == "tga") return true;

    qDebug() << ext;

    return false;
}

QString ConversationController::renderMessage(const QString &message) {
    QRegExp url(".*(http[s]*://[^ ]+).*");
    //url.setMinimal(true);

    int pos = 0;
    int lastPos = 0;
    QString nMessage;

    while((pos = url.indexIn(message, lastPos)) != -1) {
        nMessage += message.mid(lastPos, pos-lastPos);

        //if(isImage(url.cap(1))) {
        //    nMessage += "<img style=\"position: relative; right: -64px; \" src=\"" + url.cap(1) + "\" />";
        //} else {
            nMessage += "<a href=\"" + url.cap(1) + "\">" + url.cap(1) + "</a>";
        //}

        lastPos = pos + url.matchedLength();
    }
    nMessage += message.mid(lastPos);

    return nMessage;

}


void ConversationController::pushMessage(const QString &from, const QString &message) {
    if(m_WebView == NULL) {
        qWarning() << "did not received the webview. quit.";
        return;
    }

    qDebug() << "push message...";

    QString ownAvatar = ConversationManager::get()->getAvatar();
    if(ownAvatar.mid(0,9).toLower() == "asset:///")
        ownAvatar = QDir::currentPath() + "/app/native/assets/" +  ownAvatar.mid(9);


    bool ownMessage = from.toLower() == ConversationManager::get()->getUser().toLower();

    if(ownMessage)
        m_WebView->evaluateJavaScript("pushMessage(1, \"" + renderMessage(message) +"\", \"file:///" + ownAvatar + ".square.png" + "\");");
    else
        m_WebView->evaluateJavaScript("pushMessage(0, \"" + renderMessage(message) +"\", \"file:///" + m_DstAvatar + ".square.png\");");
}


void ConversationController::send(const QString& message) {
    qDebug() << "CALL!";
    if(message.isEmpty())
        return;

    ConversationManager::get()->sendMessage(message);

    if(m_WebView == NULL) {
        qWarning() << "did not received the webview. quit.";
        return;
    }

    QString ownAvatar = ConversationManager::get()->getAvatar();
    if(ownAvatar.mid(0,9).toLower() == "asset:///")
        ownAvatar = QDir::currentPath() + "/app/native/assets/" +  ownAvatar.mid(9);



    m_WebView->evaluateJavaScript("pushMessage(1, \"" + renderMessage(message) +"\", \"file:///" + ownAvatar + ".square.png" + "\");");
}
