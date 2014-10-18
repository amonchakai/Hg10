import bb.cascades 1.2
import com.netimage 1.0
import Network.ConversationController 1.0

Page {
    property string name
    property string avatar
    property string id
    property variant previewPage
    
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
                        color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.White : Color.Black
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
                settings.zoomToFitEnabled: false
                
                html: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "<!DOCTYPE html><html><head><style>body { background-color: #000000; } </style></head><body></body></html>" : "" ;
                
                onLoadingChanged: {
                    if (loadRequest.status == WebLoadStatus.Succeeded) {
                        messageView.evaluateJavaScript("scrollToEnd();");
                    }
                }
                
                onMessageReceived: {
                    var isScroll = RegExp("SCROLLTO:([0-9]+)")
                    var match = message.data.match(isScroll);
                    if(match)
                        scrollView.scrollToPoint(0, match[1], ScrollAnimation.None);
                    
                    
                }
                
                onNavigationRequested: {
                    if(request.navigationType != WebNavigationType.Other) {
                        request.action = WebNavigationRequestAction.Ignore;
                        
                        var urlImg = RegExp(".jpg");
                        var urlImgPng = RegExp(".png");
                        var urlImgGif = RegExp(".gif");
                        if(urlImg.test(request.url.toString()) || urlImgPng.test(request.url.toString()) || urlImgGif.test(request.url.toString()))
                            showPictureViewer(request.url);
                        else
                            linkInvocation.query.uri = request.url;
                    
                    } else { 
                        request.action = WebNavigationRequestAction.Accept;
                    }
                }
            }
        }
        
        
        TextField {
            preferredHeight: 30
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Fill
            id: txtField
            inputMode: TextFieldInputMode.Chat
            
            content {
                flags: TextContentFlag.Emoticons
            }
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
        },
        ActionItem {
            title: qsTr("To last message")
            imageSource: "asset:///images/icon_bottom.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                messageView.evaluateJavaScript("scrollToEnd();")
            }
        }
    ]
    
    onCreationCompleted: {
        conversationController.setWebView(messageView);
    }
    
    onIdChanged: {
        conversationController.load(id, avatar);
    }
    
    function showPictureViewer(imageUrl) {
        if(!previewPage)
            previewPage = imagePreview.createObject();
        previewPage.imageList = imageUrl;
        nav.push(previewPage);
    }
    
    attachedObjects: [
        ConversationController {
            id: conversationController
        },
        Invocation {
            id: linkInvocation
                    
            query.invokeTargetId: "sys.browser";
            query.invokeActionId: "bb.action.OPEN"
                    
                    
            query {
                   onUriChanged: {
                        linkInvocation.query.updateQuery();
                        //linkInvocation.query.invokeTargetId = "sys.browser";
                        //linkInvocation.query.mimeType = "text/html";
                }
            }
                    
            onArmed: {
                        
                  trigger("bb.action.OPEN");
            }
        },
        ComponentDefinition {
            id: imagePreview
            source: "ImagePreview.qml"
        }
    ]
}
