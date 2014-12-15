import bb.cascades 1.2
import com.netimage 1.0
import bb.cascades.pickers 1.0
import Network.ConversationController 1.0
import bb.multimedia 1.2

Page {
    property string name
    property string avatar
    property string id
    property bool   room
    property variant previewPage
    property variant smileyPage
    property string smileyToAdd
    property string filenameChat
    
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
                    minHeight: 60
                    maxHeight: 60
                    minWidth: 60
                    maxWidth: 60
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
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_left.png" : "asset:///images/icon_left_black.png"
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
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            ActivityIndicator {
                id: linkStatusActivity
                preferredHeight: 50
                horizontalAlignment: HorizontalAlignment.Center
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
                        
                        var isOpenImg = RegExp("OPEN_IMAGE:([^\']+)")
                        match = message.data.match(isOpenImg);
                        if(match)
                            showPictureViewer(match[1]);
                        
                        var isOpenSound = RegExp("PLAY_SOUND:([^\']+)")
                        match = message.data.match(isOpenSound);
                        if(match) {
                            audioPlayer.sourceUrl = "file:///" + match[1];
                            audioPlayer.play();
                        }
                        
                        
                        
                        console.log(message.data)
                    }
                    
                    onNavigationRequested: {
                        if(request.navigationType != WebNavigationType.Other) {
                            request.action = WebNavigationRequestAction.Ignore;
                            
                            console.log("on nav requested")
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
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#202020") : Color.create("#f5f5f5")
                    
                    
                    ImageButton {
                        preferredHeight: 40
                        preferredWidth: 35
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/sound_white.png" : "asset:///images/sound.png"
                        verticalAlignment: VerticalAlignment.Center
                        
                        onTouch: {
                            if(event.isUp()) {
                                console.log("stop record");
                                voiceRecording.visible = false;
                                if(recorder.mediaState != MediaState.Unprepared) {
                                    recorder.reset();
                                    conversationController.sendData(filenameChat);
                                }
                            }
                            if(event.isDown()) {
                                console.log("record");
                                voiceRecording.visible = true;
                                if(!conversationController.fileReady) {
                                    filenameChat = conversationController.nextAudioFile;
                                    recorder.setOutputUrl(filenameChat);
                                }
                                
                                recorder.record();
                            }
                        }   
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
                    
                    ImageButton {
                        verticalAlignment: VerticalAlignment.Center
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/whiteFace.png" : "asset:///images/blackFace.png"
                        preferredHeight: 40
                        preferredWidth: 40
                        onClicked: {
                            toogleEmoji();
                        }
                    }
                    Container {
                        preferredWidth: 10
                        background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#202020") : Color.create("#f5f5f5")
                    }
                }
                
                Container {
                    id: emoticonsPicker
                    preferredHeight: 0
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    
                    attachedObjects: [
                        ImplicitAnimationController {
                            id: animList
                            propertyName: "preferredHeight"
                        }
                    ]
                    
                    
                    ListView {
                        layout: GridListLayout {
                            columnCount: 9
                        }
                        dataModel: XmlDataModel {
                            source: "asset:///data/emojies.xml"
                        }
                        
                        listItemComponents: [
                            ListItemComponent {
                                type: "item"
                                
                                Container {
                                    
                                    Label {
                                        content.flags: TextContentFlag.Emoticons
                                        text: ListItemData.name
                                        textStyle.fontSize: FontSize.XLarge
                                    }                        
                                }
                            }
                        ]
                        
                        onTriggered: {
                            var chosenItem = dataModel.data(indexPath);
                            txtField.text = txtField.text  + chosenItem.name;
                            toogleEmoji();
                        }
                    }    
                }
            }
        }
        
        Container {
            id: voiceRecording
            visible: false
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            ImageView {
                preferredHeight: 200
                preferredWidth: 200
                imageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/walkie_white.png" : "asset:///images/walkie.png"
                verticalAlignment: VerticalAlignment.Center
            }
        }
    
    }
    
    function toogleEmoji() {
        if(emoticonsPicker.preferredHeight == 0) {
            emoticonsPicker.preferredHeight=200;
        } else {
            emoticonsPicker.preferredHeight=0;
        }
    }
    
    
    actions: [
        ActionItem {
            title: qsTr("Refresh")
            imageSource: "asset:///images/icon_refresh.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                conversationController.refreshHistory(id, avatar, name);
            }
        }, 
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
            imageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/whiteFace.png" : "asset:///images/whiteFace2.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                toogleEmoji();
            }
        },
        ActionItem {
            title: qsTr("Stickers")
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///images/document.png"
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
        conversationController.setLinkActivity(linkStatusActivity);
        filenameChat = conversationController.nextAudioFile;
        recorder.setOutputUrl(filenameChat);
        recorder.prepare();
    }
    
    onIdChanged: {
        if(id != "")
            conversationController.load(id, avatar, name);
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
        AudioRecorder {
            id: recorder
        },
        MediaPlayer {
            id: audioPlayer
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
