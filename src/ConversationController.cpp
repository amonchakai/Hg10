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

ConversationController::ConversationController(QObject *parent) : QObject(parent), m_WebView(NULL), m_HistoryCleared(false) {

    bool check = connect(ConversationManager::get(), SIGNAL(historyLoaded()), this, SLOT(updateView()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = connect(ConversationManager::get(), SIGNAL(messageReceived(const QString &, const QString &)), this, SLOT(pushMessage(const QString &, const QString &)));
    Q_ASSERT(check);

    check = connect(ConversationManager::get(), SIGNAL(chatStateNotify(int)), this, SLOT(chatStateUpdate(int)));
    Q_ASSERT(check);

    check = connect(ConversationManager::get(), SIGNAL(historyMessage(QString, QString)), this, SLOT(pushHistory(QString, QString)));
    Q_ASSERT(check);
}

bool ConversationController::isOwnMessage(const QString &from) {
    qDebug() << from;

    if(from.toLower() == ConversationManager::get()->getUser().toLower()) {
        return true;
    }

    if(ConversationManager::get()->isAdressee(from))
        return false;
    else
        return true;

}

void ConversationController::load(const QString &id, const QString &avatar) {
    if(avatar.mid(0,9).toLower() == "asset:///")
        m_DstAvatar = QDir::currentPath() + "/app/native/assets/" +  avatar.mid(9);
    else
        m_DstAvatar = avatar;

    m_HistoryCleared = false;
    ConversationManager::get()->load(id);
}

void ConversationController::updateView() {
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

    QString ownAvatar = ConversationManager::get()->getAvatar();
    if(ownAvatar.mid(0,9).toLower() == "asset:///")
        ownAvatar = QDir::currentPath() + "/app/native/assets/" +  ownAvatar.mid(9);


    if (htmlTemplateFile.open(QIODevice::ReadOnly) && htmlEndTemplateFile.open(QIODevice::ReadOnly)) {
        QString htmlTemplate = htmlTemplateFile.readAll();
        QString endTemplate = htmlEndTemplateFile.readAll();

        if(settings.value("fontSize").value<int>() != 25) {
            htmlTemplate.replace("font-size: 25px;", "font-size: " + QString::number(settings.value("fontSize").value<int>()) + "px;");
        }

        const History& history = ConversationManager::get()->getHistory();

        QString body;
        for(int i = std::max(0, history.m_History.size()-10) ; i < history.m_History.size() ; ++i) {
            const TimeEvent &e = history.m_History.at(i);

            if(isOwnMessage(e.m_Who)) {
                body +=  QString("<div class=\"bubble-right\"><div class=\"bubble-right-avatar\"><img src=\"file:///" + ownAvatar + ".square.png" + "\" /></div>")
                                   + "<p>" + renderMessage(e.m_What) + "</p>"
                               + "</div>";

            } else {
                body +=  QString("<div class=\"bubble-left\"><div class=\"bubble-left-avatar\"><img src=\"file:///" + m_DstAvatar + ".square.png" + "\" /></div>")
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

    return false;
}

QString ConversationController::renderMessage(const QString &message, bool showImg) {
    QRegExp url(".*(http[s]*://[^ ]+).*");
    //url.setMinimal(true);
    url.setCaseSensitivity(Qt::CaseInsensitive);

    int pos = 0;
    int lastPos = 0;
    QString nMessage;

    while((pos = url.indexIn(message, lastPos)) != -1) {
        nMessage += message.mid(lastPos, pos-lastPos);

        if(showImg && isImage(url.cap(1))) {
            nMessage += "<img src=\"" + url.cap(1) + "\" />";
        } else {
            nMessage += "<a href=\"" + url.cap(1) + "\">" + url.cap(1) + "</a>";
        }

        lastPos = pos + url.matchedLength();
    }
    nMessage += message.mid(lastPos);

    nMessage.replace("\\n", "<br/>");

    return nMessage;

}


void ConversationController::pushMessage(const QString &from, const QString &message) {
    if(m_WebView == NULL) {
        qWarning() << "did not received the webview. quit.";
        return;
    }

    qDebug() << "push message via JS...";

    QString ownAvatar = ConversationManager::get()->getAvatar();
    if(ownAvatar.mid(0,9).toLower() == "asset:///")
        ownAvatar = QDir::currentPath() + "/app/native/assets/" +  ownAvatar.mid(9);


    bool ownMessage = isOwnMessage(from);
    QString lmessage = renderMessage(message, true);
    lmessage.replace("\"","\\\"");

    if(ownMessage)
        m_WebView->evaluateJavaScript("pushMessage(1, \"" + lmessage +"\", \"file:///" + ownAvatar + ".square.png" + "\");");
    else
        m_WebView->evaluateJavaScript("pushMessage(0, \"" + lmessage +"\", \"file:///" + m_DstAvatar + ".square.png\");");
}



void ConversationController::pushHistory(const QString &from, const QString &message) {
    if(m_WebView == NULL) {
        qWarning() << "did not received the webview. quit.";
        return;
    }

    if(!m_HistoryCleared) {
        m_HistoryCleared = true;
        m_WebView->evaluateJavaScript("clearHistory();");
    }

    qDebug() << "push message...";

    QString ownAvatar = ConversationManager::get()->getAvatar();
    if(ownAvatar.mid(0,9).toLower() == "asset:///")
        ownAvatar = QDir::currentPath() + "/app/native/assets/" +  ownAvatar.mid(9);


    bool ownMessage = isOwnMessage(from);
    QString lmessage = renderMessage(message, true);
    lmessage.replace("\"","\\\"");

    if(ownMessage)
        m_WebView->evaluateJavaScript("pushHistory(0, 1, \"" + lmessage +"\", \"file:///" + ownAvatar + ".square.png" + "\");");
    else
        m_WebView->evaluateJavaScript("pushHistory(0, 0, \"" + lmessage +"\", \"file:///" + m_DstAvatar + ".square.png\");");
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

    QString lmessage = renderMessage(message, true);
    lmessage.replace("\"","\\\"");

    m_WebView->evaluateJavaScript("pushMessage(1, \"" + lmessage +"\", \"file:///" + ownAvatar + ".square.png" + "\");");
}


void ConversationController::sendData(const QString &file) {
    ConversationManager::get()->sendData(file);
}


void ConversationController::chatStateUpdate(int state) {
    m_WebView->evaluateJavaScript("chatStateUpdate(" + QString::number(state) + ");");
}
