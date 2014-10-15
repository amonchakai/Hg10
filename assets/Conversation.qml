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
            id: scrollView
            
            WebView {
                id: messageView
                
                settings.textAutosizingEnabled: false
                
                onLoadingChanged: {
                    if (loadRequest.status == WebLoadStatus.Succeeded) {
                        messageView.evaluateJavaScript("scrollToEnd();")
                    }
                }
                
                onMessageReceived: {
                    var isScroll = RegExp("SCROLLTO:([0-9]+)")
                    var match = message.data.match(isScroll);
                    if(match)
                        scrollView.scrollToPoint(0, match[1], ScrollAnimation.None);
                    
                    
                }
            }
        }
        
        
        TextField {
            preferredHeight: 30
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Fill
            id: txtField
        }
        
    }
    
    
    actions: [
        ActionItem {
            title: qsTr("Reply")
            imageSource: "asset:///images/send.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                conversationController.send(txtField.text);
                txtField.text = "";            
            }
        }
    ]
    
    onCreationCompleted: {
        conversationController.setWebView(messageView);
    }
    
    onIdChanged: {
        conversationController.load(id, avatar);
    }
    
    
    attachedObjects: [
        ConversationController {
            id: conversationController
        }
    ]
}
