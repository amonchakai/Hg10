import bb.cascades 1.2
import com.netimage 1.0
import Network.RoomController 1.0


NavigationPane {
    id: nav
    property variant tpage
    property int depth
    property variant createRoomPage
    
    property string newParticipants
    
    Page {
        
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
                        horizontalAlignment: HorizontalAlignment.Left
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
                                
                                source: roomController.avatar                                    
                            } 
                        ]
                    }
                    
                    Label {
                        text: qsTr("Rooms")
                        textStyle {
                            color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.White : Color.Black
                        }
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    
                    ImageButton {
                        id: newRoomButton
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        defaultImageSource: "asset:///images/icon_chat.png"
                        onClicked: {
                            if(!createRoomPage)
                                createRoomPage = createRoom.createObject();
                            nav.push(createRoomPage);
                        }
                    }
                }
            }
        }
        
        Container {
            ListView {
                id: roomList
                
                dataModel: GroupDataModel {
                    id: theModel
                    grouping: ItemGrouping.None
                }
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        
                        Container {
                            id: overallContactContainer
                            layout: StackLayout {
                                orientation: LayoutOrientation.TopToBottom
                            }
                            
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                preferredHeight: 110
                                
                                ImageView {
                                    verticalAlignment: VerticalAlignment.Center
                                    id: avatarImg
                                    scalingMethod: ScalingMethod.AspectFit
                                    minHeight: 90
                                    maxHeight: 90
                                    minWidth: 90
                                    maxWidth: 90
                                    image: tracker.image
                                    
                                    attachedObjects: [
                                        NetImageTracker {
                                            id: tracker
                                            
                                            source: ListItemData.avatar                                    
                                        } 
                                    ]
                                }
                                
                                Label {
                                    id: name
                                    text: ListItemData.name
                                    verticalAlignment: VerticalAlignment.Center
                                }
                            }
                            
                            Divider {
                            
                            }
                            
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Contact")
                                    
                                    DeleteActionItem {
                                        title: qsTr("Clear history")
                                        onTriggered: {
                                            overallContactContainer.ListItem.view.deleteHistory(ListItemData.id)
                                        }
                                    }
                                }
                            ]
                        }
                    }
                
                ]
                function deleteHistory(id) {
                    roomController.deleteRoom(id);
                }
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    
                    // Create the content page and push it on top to drill down to it.
                    if(!tpage) {
                        tpage = conversation.createObject();
                    }
                    
                    // Set the url of the page to load and thread caption. 
                    tpage.name     = chosenItem.name;
                    tpage.avatar   = chosenItem.avatar;
                    tpage.id       = chosenItem.id;
                    tpage.room     = true;
                    
                    nav.push(tpage);
                    
                }
            }
        }
        
        onCreationCompleted: {
            roomController.setListView(roomList);
            roomController.updateView();
        }
        
        
        
        attachedObjects: [
            RoomController {
                id: roomController
            },
            ComponentDefinition {
                id: createRoom
                source: "CreateRoom.qml"
            },
            ComponentDefinition {
                id: conversation
                source: "Conversation.qml"
            }
        ]
        
    }
    
    onNewParticipantsChanged: {
        if(newParticipants == "")
            return;
            
        roomController.createRoom(newParticipants);
        newParticipants = "";
    }
}
