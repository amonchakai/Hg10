import bb.cascades 1.2
import Network.DropBoxConnectController 1.0


Page {
    
    titleBar: TitleBar {
        title: qsTr("Dropbox")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                navSettings.pop();
            }
        }
        
        acceptAction: ActionItem {
            title: qsTr("Save")
            onTriggered: {
                dropboxConnect.getToken(accessCode.text);
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
                id: dropboxConnectView
                
                settings.textAutosizingEnabled: false
                settings.zoomToFitEnabled: false
            
            }
        
        }
        
        TextField {
            id: accessCode
            hintText: qsTr("Insert access code")
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Fill
        }
        
        attachedObjects: [
            DropBoxConnectController {
                id: dropboxConnect
                
                onCloseConnect: {
                    navSettings.pop();
                }
            }
        ]
        
        onCreationCompleted: {
            dropboxConnect.setWebView(dropboxConnectView);
            dropboxConnect.logInRequest();
        }
    }
}
