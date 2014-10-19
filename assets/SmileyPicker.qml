import bb.cascades 1.2
import Network.SmileyPickerController 1.0

Page {
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        verticalAlignment: VerticalAlignment.Fill
        
        ScrollView {    
            id: scrollView
            rightMargin: 50
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            WebView {
                id: smileyList
                settings.textAutosizingEnabled: false
    //            layoutProperties: StackLayoutProperties {
    //                spaceQuota: 1
    //            }
                
                onMessageReceived: {
                    smileyToAdd = message.data;
                    nav.pop();
                }
            }
        }
    }
    
    attachedObjects: [
    	SmileyPickerController {
            id: smileyPickerController

        }    
    ]
    
    onCreationCompleted: {
        smileyPickerController.setWebView(smileyList);
        smileyPickerController.loadDefautSmiley();
        
    }
}
