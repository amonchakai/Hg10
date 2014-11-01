import bb.cascades 1.2
import com.netimage 1.0
import Network.RoomController 1.0


NavigationPane {
    id: nav
    property variant tpage
    property int depth
    property variant createRoomPage
    
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
            }
        }
        
        onCreationCompleted: {
            roomController.setListView(roomList);
        }
        
        attachedObjects: [
            RoomController {
                id: roomController
            },
            ComponentDefinition {
                id: createRoom
                source: "CreateRoom.qml"
            }
        ]
        
    }
}
