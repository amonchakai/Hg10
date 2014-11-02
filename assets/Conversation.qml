import bb.cascades 1.2
import com.netimage 1.0
import bb.cascades.pickers 1.0
import Network.ConversationController 1.0

Page {
    property string name
    property string avatar
    property string id
    property bool   room
    property variant previewPage
    property variant smileyPage
    property string smileyToAdd
    
    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: DockLayout { }
                leftPadding: 10
                rightPadding: 10
                background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#282828") : Color.create("#f0f0f0");
                                
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
                
                ImageButton {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                    defaultImageSource: "asset:///images/icon_left.png"
                    onClicked: {
                        nav.pop();
                    }
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
            
            focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
            
            WebView {
                id: messageView
                
                settings.textAutosizingEnabled: false
                settings.zoomToFitEnabled: false
                
                html: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "<!DOCTYPE html><html><head><style>body { background-color: #000000; } </style></head><body></body></html>" : "" ;
                
                onLoadingChanged: {
                    if (loadRequest.status == WebLoadStatus.Succeeded) {
                        messageView.evaluateJavaScript("scrollToEnd();");
                        scrollView.requestFocus();
                    }
                }
                
                onMessageReceived: {
                    var isScroll = RegExp("SCROLLTO:([0-9]+)")
                    var match = message.data.match(isScroll);
                    if(match)
                        scrollView.scrollToPoint(0, match[1], ScrollAnimation.None);
                    
                    console.log(message.data)
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
        
        Container {
            id: inputContainer
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Fill
            
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            ProgressIndicator {
                horizontalAlignment: HorizontalAlignment.Fill
                id: uploadingIndicator
                fromValue: 0
                toValue: 100
                visible: false
            }
            
            TextField {
                preferredHeight: 30
                horizontalAlignment: HorizontalAlignment.Fill
                id: txtField
                inputMode: TextFieldInputMode.Chat
                
                input {
                    submitKey: SubmitKey.Send
                    onSubmitted: {
                        conversationController.send(txtField.text);
                        txtField.text = "";  
                    }
                }
                content {
                    flags: TextContentFlag.Emoticons
                }
            }
        }
        
    }
    
    
    actions: [
        ActionItem {
            title: qsTr("Attach")
            imageSource: "asset:///images/icon_attach.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                filePicker.open();
            }
        }, 
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
            title: qsTr("Emoticons")
            imageSource: "asset:///images/whiteFace.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if(!smileyPage)
                    smileyPage = smileyPicker.createObject();
                nav.push(smileyPage);
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
        if(id != "")
            conversationController.load(id, avatar);
    }
    
    onSmileyToAddChanged: {
        if(smileyToAdd != "")
            txtField.text = txtField.text + " " + smileyToAdd;
        smileyToAdd = "";
    }
    
    onRoomChanged: {
        conversationController.isRoom = room;
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
            
            onReceivedUrl: {
                uploadingIndicator.visible = false;
                txtField.text = txtField.text + " " + url;
            }
            
            onUploading: {
                uploadingIndicator.visible = true;
                uploadingIndicator.value = status;
            }
            
            onComplete: {
                scrollView.requestFocus();
            }
        },
        Invocation {
            id: linkInvocation
                    
            query.invokeTargetId: "sys.browser";
            query.invokeActionId: "bb.action.OPEN"
                    
                    
            query {
                   onUriChanged: {
                        linkInvocation.query.updateQuery();
                }
            }
                    
            onArmed: {
                        
                  trigger("bb.action.OPEN");
            }
        },
        FilePicker {
            id:filePicker
            type : FileType.Picture
            title : "Select Picture"
            directories : ["/accounts/1000/shared/misc"]
            onFileSelected : {
                if(selectedFiles.length > 0) {
                    conversationController.sendData(selectedFiles[0]);
                }
            }
        },
        ComponentDefinition {
            id: imagePreview
            source: "ImagePreview.qml"
        },
        ComponentDefinition {
            id: smileyPicker
            source: "SmileyPicker.qml"
        }
    ]
}
