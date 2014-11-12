import bb.cascades 1.2
import Network.FacebookController 1.0

Page {
    
    titleBar: TitleBar {
        title: qsTr("Facebook")
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
                id: facebookConnectView
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                minHeight: 720
                
                settings.textAutosizingEnabled: false
                settings.zoomToFitEnabled: false
                
            }
            
        }
        
        attachedObjects: [
            FacebookController {
                id: facebookController
                
                onCloseConnect: {
                    navSettings.pop();
                }
                
            }
        ]
        
        onCreationCompleted: {
            facebookController.setWebView(facebookConnectView);
            facebookController.logInRequest();
        }
    }
}
