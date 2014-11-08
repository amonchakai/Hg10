import bb.cascades 1.3
import Network.ListContactsController 1.0
import com.netimage 1.0

NavigationPane {
    id: nav
    property variant tpage
    property int depth
        
    Page {
        
        titleBar: TitleBar {
            kind: TitleBarKind.FreeForm
            kindProperties: FreeFormTitleBarKindProperties {
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    leftPadding: 10
                    rightPadding: 10
                    background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#282828") : Color.create("#f0f0f0");
                    
                    ImageView {
                        verticalAlignment: VerticalAlignment.Center
                        //horizontalAlignment: HorizontalAlignment.Left
                        id: avatarOwnImg
                        scalingMethod: ScalingMethod.AspectFit
                        minHeight: ui.sdu(9)
                        maxHeight: ui.sdu(9)
                        minWidth: ui.sdu(9)
                        maxWidth: ui.sdu(9)
                        image: trackerOwn.image
                        
                        attachedObjects: [
                            NetImageTracker {
                                id: trackerOwn
                                
                                source: listContactsController.avatar                                    
                            } 
                        ]
                    }
                    
                    Label {
                        text: listContactsController.userName
                        textStyle {
                            color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.White : Color.Black
                        }
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties { spaceQuota: 1 }
                    }
                }
            }
        }
        
        Container {
            
            layout: StackLayout {
            
            }
            
            ActivityIndicator {
                id: connectingActivity
                preferredHeight: ui.sdu(6)
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top
            }
            TextField {
                id: contactFilter
                visible: false
                hintText: qsTr("Search a contact")
                onTextChanging: {
                    if(text.length > 3) {
                        listContactsController.filter(text);
                    }
                    if(text.length == 0) {
                        listContactsController.updateView();
                        visible = false;
                    }
                }
            }
            ListView {
                signal refreshTriggered()
                property bool loading: false
                leadingVisualSnapThreshold: 2.0
                leadingVisual: RefreshHeader {
                    id: refreshHandler
                    onRefreshTriggered: {
                        listContactView.refreshTriggered();
                    }
                }
                onTouch: {
                    refreshHandler.onListViewTouch(event);
                }
                onLoadingChanged: {
                    refreshHandler.refreshing = refreshableList.loading;
                    
                    if(!refreshHandler.refreshing) {
                        // If the refresh is done 
                        // Force scroll to top to ensure that all items are visible
                        scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
                    }
                }
                
                
                onRefreshTriggered: {
                    contactFilter.visible = true;
                }
                
                gestureHandlers: [
                    PinchHandler {
                        onPinchEnded: {
                            listContactsController.setFilter(event.pinchRatio < 1);
                        }
                    }
                ]
                
                id: listContactView
                dataModel: GroupDataModel {
                    id: theModel
                    sortingKeys: ["timestamp"]
                    sortedAscending: false
                    grouping: ItemGrouping.None
                    
                    property bool empty: true
                    
                    
                    onItemAdded: {
                        empty = isEmpty();
                        listContactView.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
                    }
                    onItemRemoved: empty = isEmpty()  
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
                            verticalAlignment: VerticalAlignment.Fill
                            
                            Container {
                                minHeight: ui.sdu(1)
                                maxHeight: ui.sdu(1)
                            }
                            
                            Container {
                                preferredHeight: ui.sdu(12)
                                
                                id: titleContainer
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Center
                                
                                Container {
                                    minWidth: ui.sdu(.5)
                                    maxWidth: ui.sdu(.5)
                                }
                                
                                Container {
                                    layout: AbsoluteLayout { }
                                    minHeight: ui.sdu(12)
                                    maxHeight: ui.sdu(12)
                                    minWidth: ui.sdu(12)
                                    maxWidth: ui.sdu(12)
                                    
                                    //  Avatar
                                    ImageView {
                                        verticalAlignment: VerticalAlignment.Center
                                        //horizontalAlignment: HorizontalAlignment.Left
                                        id: avatarImg
                                        scalingMethod: ScalingMethod.AspectFit
                                        minHeight: ui.sdu(12)
                                        maxHeight: ui.sdu(12)
                                        minWidth: ui.sdu(12)
                                        maxWidth: ui.sdu(12)
                                        image: tracker.image
                                        
                                        attachedObjects: [
                                            NetImageTracker {
                                                id: tracker
                                                
                                                source: ListItemData.avatar                                    
                                            } 
                                        ]
                                    
                                    }
                                    
                                    ImageView {
                                        imageSource: "asset:///images/available.png"
                                        minHeight: ui.sdu(2)
                                        maxHeight: ui.sdu(2)
                                        minWidth: ui.sdu(2)
                                        maxWidth: ui.sdu(2)
                                        layoutProperties: AbsoluteLayoutProperties {
                                            positionX: ui.sdu(7)
                                            positionY: ui.sdu(7)
                                        }
                                        // 0 => online, 1 => away, 2 => away (long time), 3 => do not disturb, 4 => actively interested into chatting, 
                                        visible: ListItemData.presence > 0
                                    }
                                
                                }
                                
                                Container {
                                    minWidth: ui.sdu(3)
                                    maxWidth: ui.sdu(3)
                                    //horizontalAlignment: HorizontalAlignment.Left
                                
                                }
                                
                                Container {
                                    id: contactContainer
                                    preferredWidth: ui.sdu(200)
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.TopToBottom
                                    }
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Center
                                    
                                    Container {
                                        layout: DockLayout {
                                        }
                                        horizontalAlignment: HorizontalAlignment.Fill
                                        verticalAlignment: VerticalAlignment.Center
                                        
                                        Label {
                                            text: ListItemData.name
                                        }
                                        
                                        Label {
                                            text: ListItemData.timestampString
                                            
                                            horizontalAlignment: HorizontalAlignment.Right
                                            verticalAlignment: VerticalAlignment.Center
                                            textStyle {
                                                base: SystemDefaults.TextStyles.SmallText
                                                color: (ListItemData.read == 1) ? Color.Gray : (Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#00a8df") : Color.Blue);
                                            }
                                        }
                                    }
                                    
                                    
                                    Container {
                                        layout: DockLayout {
                                        }
                                        horizontalAlignment: HorizontalAlignment.Fill
                                        verticalAlignment: VerticalAlignment.Bottom
                                        
                                        
                                        Label {
                                            text: ListItemData.preview
                                            horizontalAlignment: HorizontalAlignment.Left
                                            textStyle {
                                                base: SystemDefaults.TextStyles.SmallText
                                                color: (ListItemData.read == 1) ? Color.Gray : (Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#00a8df") : Color.Blue);
                                            }
                                        }
                                    }
                                
                                }
                            }
                            
                            Divider { }
                            
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Contact")
                                    
                                    DeleteActionItem {
                                        title: qsTr("Remove room")
                                        onTriggered: {
                                            overallContactContainer.ListItem.view.deleteHistory(ListItemData.id);
                                            theModel.removeAt(ListItem.indexPath);
                                        }
                                    }
                                }
                            ]
                        
                        }
                    }
                ]
                
                function deleteHistory(id) {
                    listContactsController.deleteHistory(id);
                }
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    contactFilter.visible = false;
                    
                    // Create the content page and push it on top to drill down to it.
                    if(!tpage) {
                        tpage = conversation.createObject();
                    }
                    
                    // Set the url of the page to load and thread caption. 
                    tpage.name     = chosenItem.name;
                    tpage.avatar   = chosenItem.avatar;
                    tpage.id       = chosenItem.id;
                    tpage.room     = false;
                    
                    nav.push(tpage);
                }
            
            }
        }
        
        onCreationCompleted: {
            
            listContactsController.setListView(listContactView);
            listContactsController.setActivityIndicator(connectingActivity);
            listContactsController.updateView();
            
            
            depth = 0;
        }
        
        attachedObjects: [
            ListContactsController {
                id: listContactsController
                
                onCleared: {
                    if(depth > 1)
                        nav.pop();
                
                }
                
            },
            ComponentDefinition {
                id: conversation
                source: "Conversation.qml"
            }
        ]
    
    }
    
    onPushTransitionEnded: {
        ++depth;
        console.log(depth)
        
        if(depth == 1)
            listContactView.requestFocus();
    }
    
    onPopTransitionEnded: {
        --depth;
        if(depth == 1) {
            listContactView.requestFocus();
            listContactsController.markRead();
            if(tpage) 
                tpage.id = "";
        
        }
        console.log(depth)
    
    }
    
}




