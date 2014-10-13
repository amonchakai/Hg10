import bb.cascades 1.2
import com.netimage 1.0
import Network.ConversationController 1.0

Page {
    property string name
    property string avatar
    property string id
    
    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                leftPadding: 10
                rightPadding: 10
                
                ImageView {
                    verticalAlignment: VerticalAlignment.Center
                    //horizontalAlignment: HorizontalAlignment.Left
                    id: avatarOwnImg
                    scalingMethod: ScalingMethod.AspectFit
                    minHeight: 90
                    maxHeight: 90
                    minWidth: 90
                    maxWidth: 90
                    image: trackerOwn.image
                    
                    attachedObjects: [
                        NetImageTracker {
                            id: trackerOwn
                            
                            source: avatar                                    
                        } 
                    ]
                }
                
                Label {
                    text: name
                    textStyle {
                        color: Color.Black
                    }
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties { spaceQuota: 1 }
                }
            }
        }
    }
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        WebView {
            id: messageView
            layoutProperties: StackLayoutProperties { spaceQuota: 1 }
            
            settings.textAutosizingEnabled: false
        }
        
        
        TextField {
            id: message
        }
    }
    
    
    onCreationCompleted: {
        conversationController.setWebView(messageView);
    }
    
    onIdChanged: {
        conversationController.load(id);
    }
    
    
    attachedObjects: [
        ConversationController {
            id: conversationController
        }
    ]
}
