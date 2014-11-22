import bb.cascades 1.2
import Network.GoogleConnectController 1.0

Page {    
    titleBar: TitleBar {
        title: qsTr("Google")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                navSettings.pop();
            }
        }
    }
    
    Container {
        layout: DockLayout {
            
        }
        
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            id: scrollView
            
            WebView {
                id: googleConnectView
                
                settings.textAutosizingEnabled: false
                settings.zoomToFitEnabled: false
                
            }
            
        }
        
        attachedObjects: [
            GoogleConnectController {
                id: googleConnect
                
                onCloseConnect: {
                    navSettings.pop();
                    connectingActivity.start();
                }
                
                onContactInfoObtained: {
                    if(!(typeof loginController === 'undefined')) {
                        console.log("Login request -- oath2")
                        loginController.oauth2Login();
                    }

                }
            }
        ]
        
        onCreationCompleted: {
            googleConnect.setWebView(googleConnectView);
            googleConnect.logInRequest();
        }
    }
}
