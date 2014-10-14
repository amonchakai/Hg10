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
                layout: DockLayout { }
                leftPadding: 10
                rightPadding: 10
                
                ImageView {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
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
                    horizontalAlignment: HorizontalAlignment.Center
                }
                
                ImageView {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                    imageSource: "asset:///images/icon_left.png"
                }
            }
        }
    }
    
    Container {
        layout: DockLayout {
        }
        
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            
            WebView {
                
                
                id: messageView
//                layoutProperties: StackLayoutProperties { spaceQuota: 1 }
                
                settings.textAutosizingEnabled: false
            }
        }
        
        Container {
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Fill
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            TextField {
                layoutProperties: StackLayoutProperties { spaceQuota: 1 }
                id: message
            }
            ImageButton {
                defaultImageSource: "asset:///images/send.png"
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    conversationController.send(message.text);
                }
            }
            Container {
                minWidth: 4
                maxWidth: 4
            }
            
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
