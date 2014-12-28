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

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <bb/data/JsonDataAccess>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>

using namespace bb::cascades;
using namespace bb::data;
using namespace bb::system;
using bb::system::ApplicationStartupMode;

namespace bb
{
    namespace cascades
    {
        class Application;
        class LocaleHandler;
        class ApplicationStartupMode;
    }

    namespace system {
        class InvokeManager;
        class InvokeRequest;
        class InvokeTargetReply;
    }
}

class QTranslator;

/*!
 * @brief Application UI object
 *
 * Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {}


private slots:
    void onSystemLanguageChanged();
    void onInvoked(const bb::system::InvokeRequest& request);
    void closeCard();
    void initReconnect();


private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;

    bb::cascades::Application   *m_app;
    bb::cascades::NavigationPane *m_root;
    bb::cascades::QmlDocument   *m_qml;


    QSettings                   m_Settings;
    bb::system::InvokeManager*  m_InvokeManager;
    bool                        m_HeadlessStart;
    bool                        m_isCard;

    QString                     m_PageToLoadInCard;
};

#endif /* ApplicationUI_HPP_ */
