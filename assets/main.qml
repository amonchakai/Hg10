import bb.cascades 1.2
import Network.LoginController 1.0

TabbedPane {
    id: mainTab
    showTabsOnActionBar: true
    activeTab: tabHome
        
    attachedObjects: [
        
        Sheet {
            id: welcome
            Welcome {
                onDone: {
                    welcome.close();
                }
            }
        },
        Sheet {
            id: settings
            Settings {
                onDone: {
                    settings.close();
                }
            }
        },
        
        LoginController {
            id: loginController
            
         }
    ]
    
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: qsTr("About")
                imageSource: "asset:///images/icon_about.png"
                onTriggered: {
                }
            },
            ActionItem {
                title: qsTr("Settings")
                imageSource: "asset:///images/icon_settings.png"
                onTriggered: {
                    settings.open();
                }
            }
        ]
    }    

    Tab { //First tab
        // Localized text with the dynamic translation and locale updates support
        id: tabHome
        title: qsTr("Home") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_home.png"

        delegateActivationPolicy: TabDelegateActivationPolicy.Default

        delegate: Delegate {
            source: "Home.qml"
        }

    } //End of home tab


    Tab { //Favorite tab
        id: tabFav
        title: qsTr("Favorite") + Retranslate.onLocaleOrLanguageChanged
        
        imageSource: "asset:///images/icon_favorites.png"
        
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateImmediately
        
        delegate: Delegate {
            source: "Favorite.qml"
        }
    
    } //End of favorite tab
    
    onCreationCompleted: {
        if(!loginController.isLogged()) {
            welcome.open();
        }
    }
    
    onActiveTabChanged: {
        
    }
}