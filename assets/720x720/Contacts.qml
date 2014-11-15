import bb.cascades 1.2
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
                        minHeight: 60
                        maxHeight: 60
                        minWidth: 60
                        maxWidth: 60
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
                preferredHeight: 60
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
                        listContactView.scroll(-1);
                        empty = isEmpty();
                        listContactView.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
                    }
                    onItemRemoved: {
                        empty = isEmpty();
                        listContactView.scroll(-1);
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
                            verticalAlignment: VerticalAlignment.Fill
                            
                            Container {
                                minHeight: 5
                                maxHeight: 5
                            }
                            
                            Container {
                                preferredHeight: 100
                                
                                id: titleContainer
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Center
                                
                                Container {
                                    minWidth: 5
                                    maxWidth: 5
                                }
                                
                                Container {
                                    layout: AbsoluteLayout { }
                                    minHeight: 90
                                    maxHeight: 90
                                    minWidth: 90
                                    maxWidth: 90
                                    
                                    //  Avatar
                                    ImageView {
                                        verticalAlignment: VerticalAlignment.Center
                                        //horizontalAlignment: HorizontalAlignment.Left
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
                                    
                                    ImageView {
                                        imageSource: "asset:///images/available.png"
                                        minHeight: 20
                                        maxHeight: 20
                                        minWidth: 20
                                        maxWidth: 20
                                        layoutProperties: AbsoluteLayoutProperties {
                                            positionX: 70
                                            positionY: 70
                                        }
                                        // 0 => online, 1 => away, 2 => away (long time), 3 => do not disturb, 4 => actively interested into chatting, 
                                        visible: ListItemData.presence > 0
                                    }
                                
                                }
                                
                                Container {
                                    minWidth: 30
                                    maxWidth: 30
                                    //horizontalAlignment: HorizontalAlignment.Left
                                
                                }
                                
                                Container {
                                    id: contactContainer
                                    preferredWidth: 2000
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
                                        title: qsTr("Clear history")
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




