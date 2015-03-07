import bb.cascades 1.3
import com.netimage 1.0
import bb.cascades.pickers 1.0
import Network.ConversationController 1.0
import bb.multimedia 1.2
import Lib.QTimer 1.0

NavigationPane {
    id: nav
    property int depth
    
    Page {
        id: conversationCard
        objectName: "conversationCard"
        
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
                        minHeight: ui.du(9)
                        maxHeight: ui.du(9)
                        minWidth: ui.du(9)
                        maxWidth: ui.du(9)
                        image: trackerOwn.image
                        
                        attachedObjects: [
                            NetImageTracker {
                                id: trackerOwn
                                
                                source: conversationCard.avatar                                    
                            } 
                        ]
                    }
                    
                    Label {
                        id: userName
                        text: conversationCard.name
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
                            conversationController.closeCard();
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
                
                background: back.imagePaint
                
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
                            preferredHeight: 60
                            defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/sound_white.png" : "asset:///images/sound.png"
                            verticalAlignment: VerticalAlignment.Center
                            
                            onTouch: {
                                if(event.isUp()) {
                                    console.log("stop record");
                                    voiceRecording.visible = false;
                                    if(recorder.mediaState != MediaState.Unprepared) {
                                        recorder.reset();
                                        conversationController.sendAudioData(filenameChat);
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
                        
                        TextArea {
                            preferredHeight: ui.du(10)
                            horizontalAlignment: HorizontalAlignment.Fill
                            id: txtField
                            inputMode: TextAreaInputMode.Chat
                            
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
                            preferredHeight: 60
                            preferredWidth: 60
                            onClicked: {
                                conversationCard.toogleEmoji();
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
                                conversationCard.toogleEmoji();
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
                emoticonsPicker.preferredHeight=500;
                txtField.preferredHeight=50;
            } else {
                emoticonsPicker.preferredHeight=0;
                txtField.preferredHeight=ui.du(10);
            }
        }
        
        
        actions: [
            ActionItem {
                title: qsTr("Refresh")
                imageSource: "asset:///images/icon_refresh.png"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                    conversationController.refreshHistory(conversationCard.id, conversationCard.avatar, conversationCard.name);
                }
                shortcuts: [
                    Shortcut {
                        key: "r"
                    }
                ]
            }, 
            ActionItem {
                title: qsTr("Attach")
                imageSource: "asset:///images/icon_attach.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    filePicker.open();
                }
                shortcuts: [
                    Shortcut {
                        key: "a"
                    }
                ]
            },
            ActionItem {
                title: qsTr("Reply")
                imageSource: "asset:///images/send.png"
                ActionBar.placement:  ActionBarPlacement.Signature;
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
                    conversationCard.toogleEmoji();
                }
                shortcuts: [
                    Shortcut {
                        key: "e"
                    }
                ]
            },
            ActionItem {
                title: qsTr("Stickers")
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/document.png"
                onTriggered: {
                    if(!conversationCard.smileyPage)
                        conversationCard.smileyPage = smileyPicker.createObject();
                    nav.push(conversationCard.smileyPage);
                }
                shortcuts: [
                    Shortcut {
                        key: "s"
                    }
                ]
            },
            ActionItem {
                title: qsTr("Background")
                imageSource: "asset:///images/icon_wallpaper.png"
                onTriggered: {
                    conversationController.setWallpaper();
                }
            },
            ActionItem {
                title: qsTr("To last message")
                imageSource: "asset:///images/icon_bottom.png"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                    messageView.evaluateJavaScript("scrollToEnd();")
                }
                shortcuts: [
                    Shortcut {
                        key: "b"
                    }
                ]
            }
            
        ]
        
        onCreationCompleted: {
            depth = 0;
            conversationController.setWebView(messageView);
            conversationController.setLinkActivity(linkStatusActivity);
            timer.start();
        }
        
        onIdChanged: {
            console.log(id)
            console.log(name)
            console.log(avatar)
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
                onWallpaperChanged: {
                    messageView.settings.background = Color.Transparent;
                    back.imageSource = url;
                }
            },
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill
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
            QTimer {
                id: timer
                
                singleShot: true
                interval: 1500
                
                onTimeout: {
                    conversationCard.filenameChat = conversationController.nextAudioFile;
                    recorder.setOutputUrl(filenameChat);
                    recorder.prepare();
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
    
    onPopTransitionEnded: {
        --depth;
        
    }
    
    onPushTransitionEnded: {
        ++depth;
    }
}
