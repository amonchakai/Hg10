import bb.cascades 1.3
import com.netimage 1.0
import bb.cascades.pickers 1.0
import Network.ConversationController 1.0
import bb.multimedia 1.2
import Lib.QTimer 1.0

Page {
    property string name
    property string avatar
    property string id
    property bool   room
    property variant previewPage
    property variant smileyPage
    property string smileyToAdd
    property string filenameChat
    
    property int nbIcons
    
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
    
    actionBarVisibility: ChromeVisibility.Hidden

    
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
                
                /*
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
                */
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
                        flags: TextContentFlag.ActiveText | TextContentFlag.Emoticons
                        
                    }
                }
                
                
                Container {
                    preferredWidth: 10
                    background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#202020") : Color.create("#f5f5f5")
                }
            }
            
            Container {
                preferredHeight: ui.du(12)
                horizontalAlignment: HorizontalAlignment.Fill
                
                background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#262626") : Color.create("#f0f0f0")
                layout: DockLayout {
                
                }
                
                Container {
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Center
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Container {
                        preferredWidth: ui.du(2)
                        preferredHeight: ui.du(2)
                    }
                    
                    ImageButton {
                        verticalAlignment: VerticalAlignment.Center
                        
                        preferredHeight: ui.du(10)
                        preferredWidth: ui.du(10)
                        
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_left_blue_black.png" : "asset:///images/icon_left_blue.png"
                        
                        onClicked: {
                            nav.pop();
                        }
                    
                    }
                    
                    ImageButton {
                        id: actionButton
                        verticalAlignment: VerticalAlignment.Center
                        
                        preferredHeight: ui.du(8)
                        preferredWidth: ui.du(8)
                        
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_feed.png" : "asset:///images/icon_feed_black.png"
                        
                        onClicked: {
                            actionSelector.selectedOption = option1;
                            toogleEmoji();
                        }
                    }
                    
                    Label {
                        id: labelAction
                        verticalAlignment: VerticalAlignment.Center
                        text: qsTr("Actions")
                        textStyle.fontSize: FontSize.Small
                    }
                
                }  
                
                Container {
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    // Action bar actions
                    
                    
                    ImageButton {
                        verticalAlignment: VerticalAlignment.Center
                        id: composeNewActionBar
                        
                        preferredHeight: ui.du(8)
                        preferredWidth: ui.du(8)
                        
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_write_rounded.png" : "asset:///images/icon_write_rounded_black.png"
                        
                        onClicked: {
                            conversationController.send(txtField.text);
                            txtField.text = ""; 
                        }
                    }
                    
                    ImageButton {
                        verticalAlignment: VerticalAlignment.Center
                        id: smileyNewActionBar
                        
                        preferredHeight: ui.du(8)
                        preferredWidth: ui.du(8)
                        
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_redface_rounded.png" : "asset:///images/icon_redface_rounded_black.png"
                        
                        onClicked: {
                            actionSelector.selectedOption = option2;
                            toogleEmoji();
                        }
                    }
                    
                    Container {
                        preferredWidth: ui.du(2)
                        preferredHeight: ui.du(2)
                    }
                }
            }
            
            Container {
                id: emoticonsPicker
                preferredHeight: 0
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                visible: false
                
                attachedObjects: [
                    ImplicitAnimationController {
                        id: animList
                        propertyName: "preferredHeight"
                    }
                ]
                                
                SegmentedControl {
                    horizontalAlignment: HorizontalAlignment.Fill
                    id: actionSelector
                    
                    Option {
                        id: option1
                        text: qsTr("Actions")
                        value: 0
                    }
                    Option {
                        id: option2
                        text: qsTr("Smileys")
                        value: 1
                    }
                    
                    onSelectedOptionChanged: {
                        conversationController.loadActionMenu(selectedOption.value);
                    }

                }
                
                
                ListView {
                    id: actionComposerListView
                    
                    layout: GridListLayout {
                        columnCount: actionSelector.selectedOption.value == 0 ? nbIcons : 9
                        headerMode: ListHeaderMode.Sticky
                    }
                    
                    dataModel: GroupDataModel {
                        id: theModel
                        grouping: ItemGrouping.ByFullValue
                        sortingKeys: ["category"]
                        sortedAscending: false
                    
                    }
                    
                    
                    
                    listItemComponents: [
                        ListItemComponent {
                            type: "item"
                            
                            Container {
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Center
                                layout: StackLayout {
                                    orientation: LayoutOrientation.TopToBottom
                                }
                                
                                Container {
                                    preferredHeight: ui.du(2)
                                }
                                
                                ImageView {
                                    visible: ListItemData.image != ""
                                    verticalAlignment: VerticalAlignment.Center
                                    horizontalAlignment: HorizontalAlignment.Center
                                    id: avatarImg
                                    scalingMethod: ScalingMethod.AspectFit
                                    image: tracker.image
                                    minHeight: ui.du(6)
                                    maxHeight: ui.du(6)
                                    minWidth: ui.du(6)
                                    maxWidth: ui.du(6)
                                    
                                    attachedObjects: [
                                        NetImageTracker {
                                            id: tracker
                                            source: ListItemData.image                                    
                                        } 
                                    ]
                                }
                                
                                Label {
                                    text: ListItemData.caption
                                    horizontalAlignment: HorizontalAlignment.Center
                                    verticalAlignment: VerticalAlignment.Center
                                    content.flags: TextContentFlag.Emoticons
                                    textStyle.fontSize: ListItemData.image != "" ? FontSize.XSmall : FontSize.XLarge
                                }
                            
                            }
                        }
                    ]
                    
                    onTriggered: {
                        
                        var chosenItem = dataModel.data(indexPath);
                        if(chosenItem.image == "") {
                            txtField.text = txtField.text  + chosenItem.caption;
                        } else {
                            switch (chosenItem.action) {
                                case 1:
                                    messageView.evaluateJavaScript("scrollToEnd();");
                                    break;
                                
                                case 2:
                                    conversationController.refreshHistory(id, avatar, name);
                                    break;
                                
                                case 3:
                                    filePicker.open();
                                    break;
                                
                                case 4:
                                    conversationController.send(txtField.text);
                                    txtField.text = "";
                                    break;
                                
                                case 5:
                                    actionSelector.selectedOption = option2;
                                    return;
                                    
                                case 6:
                                    if(!smileyPage)
                                        smileyPage = smileyPicker.createObject();
                                    nav.push(smileyPage);
                                    break;
                                
                                case 7:
                                    conversationController.setWallpaper();
                                    break;
                                
                                 case 8:
                                     voiceRecording.visible = true;
                                     if(!conversationController.fileReady) {
                                         filenameChat = conversationController.nextAudioFile;
                                         recorder.setOutputUrl(filenameChat);
                                     }
                                     
                                     recorder.record();
                                     break;
                            }
                            
                        }
                        toogleEmoji();
                        txtField.requestFocus();
                        
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
            emoticonsPicker.visible = true;
            actionSelector.visible = true;
            emoticonsPicker.preferredHeight = DisplayInfo.width > 1000 ? 700 : 500;
            txtField.preferredHeight=50;
            
        } else {
            emoticonsPicker.preferredHeight=0;
            txtField.preferredHeight=ui.du(10);
            actionSelector.visible = false;
            emoticonsPicker.visible = false;
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
            ActionBar.placement: ActionBarPlacement.Signature
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
                if(!smileyPage)
                    smileyPage = smileyPicker.createObject();
                nav.push(smileyPage);
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
        conversationController.setWebView(messageView);
        conversationController.setLinkActivity(linkStatusActivity);
        conversationController.setActionListView(actionComposerListView);
        
        conversationController.loadActionMenu(0);
        
        timer.start();
        actionSelector.visible = false;
        nbIcons = DisplayInfo.width > 1000 ? 6 : 4;
        
    }
    
    onIdChanged: {
        if(id != "") {
            actionSelector.visible = false;
            conversationController.load(id, avatar, name);
        }
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
            directories : ["/accounts/1000/shared/"]
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
                filenameChat = conversationController.nextAudioFile;
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
