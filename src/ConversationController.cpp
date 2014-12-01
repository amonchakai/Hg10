/*
 * ConversationController.cpp
 *
 *  Created on: 13 oct. 2014
 *      Author: pierre
 */


#include "ConversationController.hpp"
#include "ConversationManager.hpp"
#include "DropBoxConnectController.hpp"
#include "XMPPService.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>

ConversationController::ConversationController(QObject *parent) : QObject(parent),
            m_WebView(NULL),
            m_LinkActivity(NULL),
            m_HistoryCleared(false),
            m_IsRoom(false),
            m_UploadingAudio(false),
            m_DropboxController(NULL) {

    bool check = connect(ConversationManager::get(), SIGNAL(historyLoaded()), this, SLOT(updateView()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = connect(ConversationManager::get(), SIGNAL(messageReceived(const QString &, const QString &)), this, SLOT(pushMessage(const QString &, const QString &)));
    Q_ASSERT(check);

    check = connect(ConversationManager::get(), SIGNAL(chatStateNotify(int)), this, SLOT(chatStateUpdate(int)));
    Q_ASSERT(check);

    check = connect(ConversationManager::get(), SIGNAL(historyMessage(QString, QString)), this, SLOT(pushHistory(QString, QString)));
    Q_ASSERT(check);

    check = connect(XMPP::get(), SIGNAL(connectedXMPP()), this, SLOT(linkEstablished()));
    Q_ASSERT(check);

    check = connect(XMPP::get(), SIGNAL(connectionFailed()), this, SLOT(waitingLink()));
    Q_ASSERT(check);

}

bool ConversationController::isOwnMessage(const QString &from) {

    if(from.toLower() == ConversationManager::get()->getUser().toLower()) {
        return true;
    }

    return false;
}

void ConversationController::waitingLink() {
    if(m_LinkActivity == NULL)
        return;

    m_LinkActivity->start();
}

void ConversationController::linkEstablished() {
    if(m_LinkActivity == NULL)
        return;

    m_LinkActivity->stop();
}

void ConversationController::load(const QString &id, const QString &avatar, const QString &name) {
    if(avatar.mid(0,9).toLower() == "asset:///")
        m_DstAvatar = QDir::currentPath() + "/app/native/assets/" +  avatar.mid(9);
    else
        m_DstAvatar = avatar;

    XMPP::get()->askConnectionStatus();

    m_HistoryCleared = false;
    ConversationManager::get()->load(id, name);
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

        if(settings.value("fontSize", 28).value<int>() != 28) {
            htmlTemplate.replace("font-size: 28px;", "font-size: " + QString::number(settings.value("fontSize").value<int>()) + "px;");
        }

        const History& history = ConversationManager::get()->getHistory();
        {
            m_AudioMessages.clear();
            QFile audioFileHist(QDir::homePath() + QLatin1String("/ApplicationData/AudioMessages/AudioHistory.txt"));
            if (audioFileHist.open(QIODevice::ReadOnly)) {
                AudioMessage message;
                QDataStream stream(&audioFileHist);
                stream >> message;
                while(!message.m_DistUrl.isEmpty()) {
                    m_AudioMessages.push_back(message);

                    message.m_DistUrl = "";
                    stream >> message;
                }

                audioFileHist.close();
            }
        }

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
        emit complete();
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
            nMessage += "<img src=\"" + url.cap(1) + "\" onclick=\"sendURL(\'OPEN_IMAGE:" + url.cap(1) + "\');\" />";
        } else {
            bool audioMessage = false;
            QString localAudioUrl;

            for(int i = m_AudioMessages.size()-1 ; i >=0 && !audioMessage && !m_AudioMessages.isEmpty() ; --i) {
                if(m_AudioMessages.at(i).m_DistUrl == url.cap(1)) {
                    audioMessage = true;
                    localAudioUrl = m_AudioMessages.at(i).m_LocalUrl;
                }
            }

            if(audioMessage) {
                QString icon = QDir::currentPath() + "/app/native/assets/images/";
                if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
                    icon += "sound_white.png";
                } else {
                    icon += "sound.png";
                }
                nMessage = QString("<img src=\"") + icon + "\" height=\"100px\" width=\"auto\" onclick=\"sendURL(\'PLAY_SOUND:"  + localAudioUrl + "\');\" />";
            } else {
                nMessage += "<a href=\"" + url.cap(1) + "\">" + url.cap(1).mid(0, 20) + "..." + "</a>";
            }
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
    // send data via XMPP
    //ConversationManager::get()->sendData(file);

    if(file.isEmpty())
        return;

    // send data to dropbox!
    if(m_DropboxController == NULL)
        initDropbox();

    m_DropboxController->putFile(file);

}


void ConversationController::chatStateUpdate(int state) {
    m_WebView->evaluateJavaScript("chatStateUpdate(" + QString::number(state) + ");");
}


QString ConversationController::getNextAudioName() {
    QString directory = QDir::homePath() + QLatin1String("/ApplicationData/AudioMessages/");
    QString name;
    if (QFile::exists(directory)) {
        QDir dir(directory);
        dir.setNameFilters(QStringList() << "*.m4a");
        dir.setFilter(QDir::Files);
        int index = 0;
        foreach(QString dirFile, dir.entryList()) {
            index = std::max(index, dirFile.mid(8,dirFile.length()-12).toInt());
        }
        name = "message_" + QString::number(index+1) + ".m4a";

    } else {
        QDir dir;
        dir.mkpath(directory);

        name = "message_1.m4a";
    }

    m_UploadingAudio = true;
    m_AudioFileName = directory + name;

    return m_AudioFileName;
}


void ConversationController::initDropbox() {
    m_DropboxController = new DropBoxConnectController(this);

    bool check = connect(m_DropboxController, SIGNAL(uploaded()), this, SLOT(uploaded()));
    Q_ASSERT(check);
    Q_UNUSED(check);

    check = connect(m_DropboxController, SIGNAL(shared(const QString &)), this, SLOT(shared(const QString &)));
    Q_ASSERT(check);

    check = connect(m_DropboxController, SIGNAL(uploading(int)), this, SLOT(fowardUploadingProcess(int)));
    Q_ASSERT(check);
}

void ConversationController::uploaded() {
    m_DropboxController->share();
}

void ConversationController::shared(const QString &url) {
    if(!m_UploadingAudio)
        emit receivedUrl(url);
    else {
        m_UploadingAudio = false;
        QString icon = QDir::currentPath() + "/app/native/assets/images/";
        if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
            icon += "sound_white.png";
        } else {
            icon += "sound.png";
        }
        QString nMessage = QString("<img src=\"") + icon + "\" height=\"100px\" width=\"auto\" onclick=\"sendURL(\'PLAY_SOUND:"  + m_AudioFileName + "\');\" />";
        nMessage.replace("\"","\\\"");

        QString ownAvatar = ConversationManager::get()->getAvatar();
        if(ownAvatar.mid(0,9).toLower() == "asset:///")
            ownAvatar = QDir::currentPath() + "/app/native/assets/" +  ownAvatar.mid(9);

        m_WebView->evaluateJavaScript("pushMessage(1, \"" + nMessage +"\", \"file:///" + ownAvatar + ".square.png" + "\");");

        emit receivedUrl("");

        ConversationManager::get()->sendMessage(url);

        QFile audioFileHist(QDir::homePath() + QLatin1String("/ApplicationData/AudioMessages/AudioHistory.txt"));
        if (audioFileHist.open(QIODevice::Append)) {
            AudioMessage message;
            message.m_DistUrl = url;
            message.m_LocalUrl = m_AudioFileName;

            QDataStream stream(&audioFileHist);
            stream << message;

            audioFileHist.close();
        }
    }


}

void ConversationController::fowardUploadingProcess(int status) {
    emit uploading(status);
}






