import bb.cascades 1.2
import Network.DriveController 1.0
import com.netimage 1.0
import bb.cascades.pickers 1.0
import bb.multimedia 1.2

NavigationPane {
    id: nav
    property variant tpage
    
    Page {
        titleBar: TitleBar {
            kind: TitleBarKind.FreeForm
            kindProperties: FreeFormTitleBarKindProperties {
                Container {
                    layout: DockLayout { }
                    leftPadding: 10
                    rightPadding: 10
                    background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#282828") : Color.create("#f0f0f0");
                    
                    Label {
                        id: folder
                        text: "My Drive"
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
                            driveController.pop();
                        }
                    }            
                }
            }
        }
        
        actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
        
        Container {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            layout: DockLayout { }
            
            Container {  
                id: dataEmptyLabel
                visible: theModel.empty //model.isEmpty() will not work  
                horizontalAlignment: HorizontalAlignment.Center  
                verticalAlignment: VerticalAlignment.Center
                
                layout: DockLayout {}
                
                Label {
                    text: connectingActivity.running ?  qsTr("Loading, please wait.") : qsTr("No files found in this folder.")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                ProgressIndicator {
                    id: uploadingIndicator
                    visible: false
                }
                ActivityIndicator {
                    id: connectingActivity
                    preferredHeight: 60
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Top
                }
                
                ListView {
                    scrollRole: ScrollRole.Main
                    id: fileListView
                    dataModel: GroupDataModel {
                        id: theModel
                        sortingKeys: ["type"]
                        sortedAscending: false
                        grouping: ItemGrouping.None
                        
                        property bool empty: true
                        
                        
                        onItemAdded: {
                            empty = isEmpty();
                        }
                        onItemRemoved: {
                            empty = isEmpty();
                        }  
                        onItemUpdated: empty = isEmpty()  
                        
                        // You might see an 'unknown signal' error  
                        // in the QML-editor, guess it's a SDK bug.  
                        onItemsChanged: empty = isEmpty()
                    
                    }
                    
                    focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                    
                    listItemComponents: [
                        ListItemComponent {
                            type: "header"
                            Header {
                                title: ListItemData
                            }
                        },
                        ListItemComponent {
                            type: "item"
                            
                            Container {
                                id: overallContactContainer
                                
                                layout: StackLayout {
                                    orientation: LayoutOrientation.TopToBottom
                                }
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Center
                                preferredHeight: 110
                                
                                Container {
                                    preferredHeight: 50
                                }
                                
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Center
                                    
                                    Container {
                                        preferredWidth: 15
                                        preferredHeight: 110
                                    }
                                    
                                    ImageView {
                                        verticalAlignment: VerticalAlignment.Center
                                        //horizontalAlignment: HorizontalAlignment.Left
                                        id: avatarImg
                                        scalingMethod: ScalingMethod.AspectFit
                                        minHeight: 40
                                        maxHeight: 40
                                        minWidth: 40
                                        maxWidth: 40
                                        image: tracker.image
                                        
                                        attachedObjects: [
                                            NetImageTracker {
                                                id: tracker
                                                
                                                source: ListItemData.iconLink                                    
                                            } 
                                        ]
                                    }
                                    
                                    Container {
                                        preferredHeight: 15
                                        preferredWidth: 15
                                    }
                                    
                                    Container {
                                        layout: DockLayout {
                                        }
                                        horizontalAlignment: HorizontalAlignment.Fill
                                        verticalAlignment: VerticalAlignment.Center
                                        preferredHeight: 110
                                        
                                        Label {
                                            text: ListItemData.title
                                            horizontalAlignment: HorizontalAlignment.Left
                                            verticalAlignment: VerticalAlignment.Top
                                        }
                                        
                                        Label {
                                            text: ListItemData.timestamp
                                            horizontalAlignment: HorizontalAlignment.Left
                                            verticalAlignment: VerticalAlignment.Bottom
                                            textStyle {
                                                base: SystemDefaults.TextStyles.SmallText
                                            }
                                        }
                                    }
                                    
                                }
                                
                                Divider { }
                                
                                contextActions: [
                                    ActionSet {
                                        title: qsTr("Actions")
                                        
                                        ActionItem {
                                            title: qsTr("Set as home folder")
                                            imageSource: "asset:///images/icon_home.png"
                                            onTriggered: {
                                                overallContactContainer.ListItem.view.setHome(ListItemData.id);
                                            }
                                        }
                                        ActionItem {
                                            title: qsTr("Share")
                                            imageSource: "asset:///images/icon_share.png"
                                            onTriggered: {
                                                overallContactContainer.ListItem.view.shareLink(ListItemData.id, ListItemData.openLink);
                                            }
                                        }
                                        ActionItem {
                                            title: qsTr("Rename")
                                            imageSource: "asset:///images/icon_rename.png"
                                            onTriggered: {
                                                overallContactContainer.ListItem.view.rename(ListItemData.id, ListItemData.title);
                                            }
                                        }
                                        ActionItem {
                                            title: qsTr("Download")
                                            imageSource: "asset:///images/icon_download.png"
                                            onTriggered: {
                                                overallContactContainer.ListItem.view.download(ListItemData.downloadLink, ListItemData.title);
                                            }
                                        }
                                        
                                    }
                                ]
                            }
                        }
                    ]
                    
                    
                    function setHome(id) {
                        driveController.setHomeFolder(id);
                    }
                    
                    function shareLink(id, link) {
                        driveController.copyShareLink(id, link);
                    }
                    
                    function rename(id, title) {
                        driveController.renameFile(id, title);
                    }
                    
                    function download(fileUrl, title) {
                        driveController.downloadFile(fileUrl, title);
                    }
                    
                    onTriggered: {
                        var chosenItem = dataModel.data(indexPath);
                                       
                        driveController.open(chosenItem.id, chosenItem.type);
                        
                        if(chosenItem.type == "application/vnd.google-apps.folder")
                            connectingActivity.start();

                        
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
                    
                    onTouch: {
                        voiceRecording.visible = false;
                        if(recorder.mediaState != MediaState.Unprepared) {
                            uploadingIndicator.visible = true;
                            recorder.reset();
                            driveController.upload(driveController.audioFile);
                        } 
                    }
                }
            }
            
        }
        
        actions: [
            ActionItem {
                id: refresh
                title: qsTr("Refresh")
                imageSource: "asset:///images/icon_refresh.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    driveController.updateView();
                }
            },
            ActionItem {
                id: createFolder
                title: qsTr("Create a new folder")
                imageSource: "asset:///images/icon_folder.png"
                onTriggered: {
                    driveController.createNewFolder();
                }
            },
            ActionItem {
                title: qsTr("Upload")
                imageSource: "asset:///images/icon_upload.png"
                onTriggered: {
                    filePicker.open();
                }
            },
            ActionItem {
                title: qsTr("Audio")
                imageSource: "asset:///images/icon_walkie.png"
                ActionBar.placement: ActionBarPlacement.Signature
                onTriggered: {
                    driveController.askName();
                }   
            }
        ]
        
        
        onCreationCompleted: {
            driveController.setListView(fileListView);
            driveController.getFileList();
            connectingActivity.start();
        }
        
        attachedObjects: [
            DriveController {
                id: driveController
                
                onComplete: {
                    connectingActivity.stop();
                    uploadingIndicator.visible = false;
                    fileListView.requestFocus();
                }
                
                onPushOpenLink: {
                    if(!tpage) {
                        tpage = webEdior.createObject();
                    }
                    
                    // Set the url of the page to load and thread caption. 
                    tpage.link     = link;
                    nav.push(tpage);
                }
                
                onUploading: {
                    uploadingIndicator.value = status;
                }
                
                onStartRecording: {
                    voiceRecording.visible = true;
                    recorder.setOutputUrl(driveController.audioFile);
                    recorder.record();
                }
            },
            ComponentDefinition {
                id: webEdior
                source: "WebEditor.qml"
            },
            AudioRecorder {
                id: recorder
            },
            FilePicker {
                id:filePicker
                type : FileType.Picture
                title : "Select a File"
                directories : ["/accounts/1000/shared/"]
                onFileSelected : {
                    if(selectedFiles.length > 0) {
                        driveController.upload(selectedFiles[0]);
                        uploadingIndicator.visible = true;
                    }
                }
            }
        ]
    }
}
