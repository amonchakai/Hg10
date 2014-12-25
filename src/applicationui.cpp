/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <QTimer>

#include "XMPPService.hpp"
#include "LoginController.hpp"
#include "ListContactsController.hpp"
#include "Image/NetImageTracker.h"
#include "ConversationController.hpp"
#include "SettingsController.hpp"
#include "GoogleConnectController.hpp"
#include "DropBoxConnectController.hpp"
#include "RoomController.hpp"
#include "SuggestContactController.hpp"
#include "SmileyPickerController.hpp"
#include "Facebook.hpp"
#include "ApplicationLogController.hpp"
#include "DriveController.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(),
        m_pLocaleHandler(NULL),
        m_app(app),
        m_root(NULL),
        m_qml(NULL),
        m_Settings("Amonchakai", "Hg10Service"),
        m_InvokeManager(new bb::system::InvokeManager(this)),
        m_HeadlessStart(false),
        m_isCard(false)
{



    bool connectResult;

    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning.
    Q_UNUSED(connectResult);
    connectResult = connect(m_InvokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(onInvoked(const bb::system::InvokeRequest&)));
    qDebug() << "HubIntegration: started and connected to invokeManager";


    // This is only available in Debug builds.
    Q_ASSERT(connectResult);

    switch(m_InvokeManager->startupMode()) {
        case bb::system::ApplicationStartupMode::LaunchApplication:
            qDebug() << "HeadlessHubIntegration: Regular application launch";
            break;
        case bb::system::ApplicationStartupMode::InvokeApplication:
            m_HeadlessStart = true;
            qDebug() << "--------------------- HeadlessHubIntegration: Launching app from invoke";
            break;
        case bb::system::ApplicationStartupMode::InvokeCard:
            m_HeadlessStart = true;
            m_isCard = true;
            qDebug() << "--------------------- HeadlessHubIntegration: Launching card from invoke";
            break;
        // enable when 10.3 beta is released
        //case ApplicationStartupMode::InvokeHeadless:
        case 4:
            qDebug() << "--------------------- HeadlessHubIntegration: Launching headless from invoke";
            m_HeadlessStart = true;
            break;
        default:
            qDebug() << "--------------------- HeadlessHubIntegration: other launch: " << m_InvokeManager->startupMode();
            break;
       }

    // ---------------------------------------------------------------------
    // Regular integration

    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);




    XMPP *client = XMPP::get();
    //client->logger()->setLoggingType(QXmppLogger::StdoutLogging);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();


    qmlRegisterType<LoginController>("Network.LoginController", 1, 0, "LoginController");
    qmlRegisterType<ListContactsController>("Network.ListContactsController", 1, 0, "ListContactsController");
    qmlRegisterType<NetImageTracker>("com.netimage", 1, 0, "NetImageTracker");
    qmlRegisterType<ConversationController>("Network.ConversationController", 1, 0, "ConversationController");
    qmlRegisterType<SettingsController>("conf.SettingsController", 1, 0, "SettingsController");
    qmlRegisterType<GoogleConnectController>("Network.GoogleConnectController", 1, 0, "GoogleConnectController");
    qmlRegisterType<DropBoxConnectController>("Network.DropBoxConnectController", 1, 0, "DropBoxConnectController");
    qmlRegisterType<RoomController>("Network.RoomController", 1, 0, "RoomController");
    qmlRegisterType<SuggestContactController>("Network.SuggestContactController", 1, 0, "SuggestContactController");
    qmlRegisterType<SmileyPickerController>("Network.SmileyPickerController", 1, 0, "SmileyPickerController");
    qmlRegisterType<Facebook>("Network.FacebookController", 1, 0, "FacebookController");
    qmlRegisterType<QTimer>("Lib.QTimer", 1, 0, "QTimer");
    qmlRegisterType<ApplicationLogController>("Lib.ApplicationLogController", 1, 0, "ApplicationLogController");
    qmlRegisterType<DriveController>("Network.DriveController", 1, 0, "DriveController");

    if(!m_HeadlessStart) {
        // Create scene document from main.qml asset, the parent is set
        // to ensure the document gets destroyed properly at shut down.
        QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

        // Create root object for the UI
        AbstractPane *root = qml->createRootObject<AbstractPane>();

        // Set created root object as the application scene
        Application::instance()->setScene(root);
    }
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("Hg10_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}



// -----------------------------------------------------------------------------------------
// receive card request


void ApplicationUI::onInvoked(const bb::system::InvokeRequest& request) {
//    qDebug() << "invoke!" << request.action();

    if(request.action().compare("bb.action.VIEW") == 0) {
//         qDebug() << "HubIntegration: onInvoked: view item: " << request.data();

         JsonDataAccess jda;

         QVariantMap objectMap = (jda.loadFromBuffer(request.data())).toMap();
         QVariantMap itemMap = objectMap["attributes"].toMap();


         QVariantList items = m_Settings.value("hub/items").toList();

         QString addresse;
         QVariantMap item;
         for(int index = 0; index < items.size(); index++) {
             item = items.at(index).toMap();
             QString sourceId = item["messageid"].toString();

              if (item["sourceId"].toString() == itemMap["messageid"].toString() ||
                  item["sourceId"].toString() == itemMap["sourceId"].toString()) {

                  addresse = item["userData"].toString();

                  break;
              }
         }

         QmlDocument *qml = QmlDocument::create("asset:///ConversationCard.qml")
                                                          .parent(this);

         m_root = qml->createRootObject<NavigationPane>();
         qml->setContextProperty("_app", this);
         m_app->setScene(m_root);

         QObject *thread = m_root->findChild<QObject*>("conversationCard");
         if(thread != NULL) {

             thread->setProperty("name", item["name"].toString());
             thread->setProperty("avatar", QDir::homePath() + QLatin1String("/vCards/") + addresse + ".png");
             thread->setProperty("room", false);
             thread->setProperty("id", addresse);


         } else
             qDebug() << "pageThread variable is not found in the qml document :(";


         InvokeRequest request;
         request.setTarget("com.amonchakai.Hg10Service");
         request.setAction("bb.action.MARKREAD");
         request.setMimeType("hub/item");
         request.setUri(QUrl("pim:"));

         QByteArray bytes;
         jda.saveToBuffer(objectMap, &bytes);
         request.setData(bytes);

         m_InvokeManager->invoke(request);

    }

    if(request.action().compare("bb.action.COMPOSE") == 0) {
        QmlDocument *qml = QmlDocument::create("asset:///StartupCardCompose.qml")
                                                                  .parent(this);

        m_root = qml->createRootObject<NavigationPane>();
        qml->setContextProperty("_app", this);
        m_app->setScene(m_root);

        QString directory = QDir::homePath() + QLatin1String("/HFRBlackData");
        if (!QFile::exists(directory)) {
            return;
        }

        QFile file(directory + "/UserID.txt");

        QString userName;
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream stream(&file);
            stream >> userName;

            file.close();
        }

        QObject *thread = m_root->findChild<QObject*>("postMesssage");
        if(thread != NULL)
            thread->setProperty("pseudo", userName);
    }


}



