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
                nav.done();
            }
        }
        acceptAction: ActionItem {
            title: qsTr("Save")
            onTriggered: {
                googleConnect.save(password.text); 
                //nav.done();
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
                
                onNavigationRequested: {
                    
                }
            }
            
        }
        
        TextField {
            id: password
            hintText: qsTr("Please provide authentification key")
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Bottom
        }
        
        
        attachedObjects: [
            GoogleConnectController {
                id: googleConnect
            }
        ]
        
        onCreationCompleted: {
            googleConnect.setWebView(googleConnectView);
            googleConnect.logInRequest();
        }
    }
}
