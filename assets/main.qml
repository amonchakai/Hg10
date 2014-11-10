import bb.cascades 1.2
import Network.LoginController 1.0
import bb.cascades 1.2

TabbedPane {
    id: mainTab
    showTabsOnActionBar: true
    activeTab: tabHome
    
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: qsTr("Policy")
                imageSource: "asset:///images/icon_about.png"
                onTriggered: {
                    about.open();
                }
            },
            ActionItem {
                title: qsTr("Settings")
                imageSource: "asset:///images/icon_settings.png"
                onTriggered: {
                    settings.open();
                }
            },
            ActionItem {
                title: loginController.notif ? " [ON]" : " [OFF]"
                imageSource: "asset:///images/icon_notification.png"
                onTriggered: {
                    loginController.notif = !loginController.notif;
                }
            
            }
        ]
    }  
    
    
    Tab { //First tab
        // Localized text with the dynamic translation and locale updates support
        id: tabHome
        title: qsTr("Contacts") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/contact.png"
        
        delegateActivationPolicy: TabDelegateActivationPolicy.Default
        
        delegate: Delegate {
            source: "Contacts.qml"
        }
    
    } //End of home tab
    
    /*
    Tab {
        id: tabRoom
        title: qsTr("Rooms")
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/chat_room.png"
        
        delegate: Delegate {
            source: "Rooms.qml"
        }

    }
    */
    
    onCreationCompleted: {
        if(!loginController.isLogged()) {
            welcome.open();
        }
    }
        
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
        Sheet {
            id: about
            Policy {
                onDone: {
                    about.close();
                }
            }
        },
        LoginController {
            id: loginController
       }
    ]
    
}




