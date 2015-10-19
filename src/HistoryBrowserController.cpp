/*
 * HistoryBrowserController.cpp
 *
 *  Created on: 18 oct. 2015
 *      Author: pierre
 */

#include "HistoryBrowserController.hpp"
#include "ConversationManager.hpp"
#include "GoogleConnectController.hpp"

#include <QRegExp>

#include <bb/cascades/Application>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Theme>
#include <bb/data/JsonDataAccess>
#include <bb/cascades/GroupDataModel>


HistoryBrowserController::HistoryBrowserController(QObject *parent) : QObject(parent), m_HistoryList(NULL), m_GoogleConnect(NULL), m_WebView(NULL) {

}


void HistoryBrowserController::getThreads(const QString& from, const QString& name, const QString &question) {
    if(m_GoogleConnect == NULL)
        m_GoogleConnect = ConversationManager::get()->getGoogleConnectController();

    if(m_GoogleConnect == NULL) {
        qWarning() << "No connection to Google";
        return;
    } else {
        bool check = QObject::connect(m_GoogleConnect, SIGNAL(rawMessageList(const QByteArray& )), this, SLOT(updateThreadsView(const QByteArray&)));
        Q_ASSERT(check);
        Q_UNUSED(check);

        check = QObject::connect(m_GoogleConnect, SIGNAL(rawMessage(const QByteArray& )), this, SLOT(updateThread(const QByteArray&)));
        Q_ASSERT(check);
    }

    QRegExp publicTalk("@public.talk.google.com");
    if(publicTalk.indexIn(from) != -1) {
        qDebug() << "m_GoogleConnect->getRawMessageList(\" from:(" + name + ") OR to:(" + name + ") \");";
        m_GoogleConnect->getRawMessageList(" from:(" + name + ") OR to:(" + name +") " + question);  // for google, user id is better, but not always available. If not, use name.
    } else {
        qDebug() << "m_GoogleConnect->getRawMessageList(\"" + from + "\");";
        m_GoogleConnect->getRawMessageList(from + " " + question);
    }

}

void HistoryBrowserController::updateThreadsView(const QByteArray& buffer) {
    using namespace bb::data;
    JsonDataAccess jda;

    QVariant qtData = jda.loadFromBuffer(buffer);

    if(jda.hasError()) {
        qDebug() << jda.error().errorMessage();
    }


    if(m_HistoryList == NULL) {
        qWarning() << "did not received the list. quit.";
        return;
    }

    using namespace bb::cascades;
    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_HistoryList->dataModel());
    dataModel->clear();

    dataModel->insertList(qtData.toMap()["threads"].toList());

    emit completed();
}




void HistoryBrowserController::loadThread(const QString& id) {
    if(m_GoogleConnect == NULL) {
        qWarning() << "No connection to Google";
        return;
    }

    m_GoogleConnect->getRawMessage(id);
}

void HistoryBrowserController::updateThread(const QByteArray& buffer) {
    using namespace bb::data;
    JsonDataAccess jda;

    QVariant qtData = jda.loadFromBuffer(buffer);

    if(jda.hasError()) {
        qDebug() << jda.error().errorMessage();
    }


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


    // -----------------------------------------------------------------------------------------------
    // customize template
    if (htmlTemplateFile.open(QIODevice::ReadOnly) && htmlEndTemplateFile.open(QIODevice::ReadOnly)) {
        QString htmlTemplate = htmlTemplateFile.readAll();
        QString endTemplate = htmlEndTemplateFile.readAll();

       // -----------------------------------------------------------------------------------------------
       // adjust font size
        if(settings.value("fontSize", 28).value<int>() != 28) {
            htmlTemplate.replace("font-size: 28px;", "font-size: " + QString::number(settings.value("fontSize").value<int>()) + "px;");
        }


       // -----------------------------------------------------------------------------------------------
       // choose background image
        {
            QString directory = QDir::homePath() + QLatin1String("/ApplicationData/Customization");
            QString filename;
            if(QFile::exists(directory + "/" + ConversationManager::get()->getAdressee() + ".xml")) {
                filename = directory + "/" + ConversationManager::get()->getAdressee() + ".xml";
            } else {
                if(QFile::exists(directory +"/default.xml")) {
                    filename = directory + "/default.xml";
                }
            }


            filename.replace(".xml", ".css");
            if(QFile::exists(filename)) {
                QFile file(filename);

                if (file.open(QIODevice::ReadOnly)) {
                    QTextStream stream(&file);
                    QString themeSettings = stream.readAll();
                    file.close();

                    QString suffix;
                    if(bb::cascades::Application::instance()->themeSupport()->theme()->colorTheme()->style() == bb::cascades::VisualStyle::Dark) {
                        suffix = "_black";
                    }

                    htmlTemplate.replace("</style><link rel=\"stylesheet\" href=\"bubble" + suffix + ".css\">", themeSettings + "\n\r</style>");

                }

            }

        }

        // -----------------------------------------------------------------------------------------------
        // preload history

        QVariantList list = qtData.toMap()["messages"].toList();

        bool fistInsertDone = false;
        QString body;
        QString prevFrom;
        for(int i = 0 ; i < list.size() ; ++i) {
            QVariantMap map = list.at(i).toMap();

            QVariantList headers = map["payload"].toMap()["headers"].toList();

            QString contact;
            if(!headers.isEmpty())
                contact = headers.at(0).toMap()["value"].toString();

            QString from;
            QString dstName;
            QRegExp fromRegExp("(.*) <(.*)>");
            if(fromRegExp.indexIn(contact) != -1) {
                from = fromRegExp.cap(2);
                dstName = fromRegExp.cap(1);
            }

            QString vCardsDir = QDir::homePath() + QLatin1String("/vCards");
            QString dstAvatar(vCardsDir + "/" + from + ".png");


            if(i > 0 && prevFrom == from && fistInsertDone) {
                body += "<li><p>" + renderMessage(map["snippet"].toString()) + "</p></li>";
            } else {
                fistInsertDone = true;
                if(i != std::max(0, list.size()))
                    body += "</ul></div><br/>";

                if(isOwnMessage(from)) {
                    body +=  QString("<div class=\"bubble-left\"><div class=\"bubble-left-avatar\"><img src=\"file:///" + ownAvatar + ".square.png" + "\" /><div class=\"author-left\"><p>" + tr("Me") +"</p></div><div class=\"time-left\"><p>" +  "</p></div></div><br/><br/><br/>")
                                       + "<ul><li><p>" + renderMessage(map["snippet"].toString()) + "</p></li>";

                } else {
                    body +=  QString("<div class=\"bubble-right\"><div class=\"bubble-right-avatar\"><img src=\"file:///" + dstAvatar + ".square.png" + "\" /><div class=\"author-right\"><p>" + dstName +"</p></div><div class=\"time-right\"><p>" +  "</p></div></div><br/><br/><br/>")
                                       + "<ul><li><p>" + renderMessage(map["snippet"].toString()) + "</p></li>";
                }
            }

            prevFrom = from;
        }
        if(!list.empty()) {
            body += "</ul></div><br/>";
        }


        m_WebView->setHtml(htmlTemplate + body  + endTemplate, "file:///" + QDir::homePath() + "/../app/native/assets/");
    }



    emit threadLoaded();
}

bool HistoryBrowserController::isOwnMessage(const QString &from) {

    if(from.toLower() == ConversationManager::get()->getUser().toLower()) {
        return true;
    }

    return false;
}


QString HistoryBrowserController::renderMessage(const QString &message, bool showImg) {
    QRegExp url("(http[s]*://[^ ]+)");
    url.setCaseSensitivity(Qt::CaseInsensitive);

    int pos = 0;
    int lastPos = 0;
    QString nMessage;

    while((pos = url.indexIn(message, lastPos)) != -1) {
        nMessage += message.mid(lastPos, pos-lastPos);

        if(showImg && isImage(url.cap(1))) {
            nMessage += "<img src=\"" + url.cap(1) + "\" onclick=\"sendURL(\'OPEN_IMAGE:" + url.cap(1) + "\');\" />";
        } else {
            nMessage += "<a href=\"" + url.cap(1) + "\">" + url.cap(1).mid(0, 20) + "..." + "</a>";
        }

        lastPos = pos + url.matchedLength();
    }
    nMessage += message.mid(lastPos);

    nMessage.replace("\\n", "<br/>");

    return nMessage;

}


bool HistoryBrowserController::isImage(const QString &url) {

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
