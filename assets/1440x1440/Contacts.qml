import bb.cascades 1.3
import bb.system 1.2
import Network.ListContactsController 1.0
import com.netimage 1.0
import Lib.QTimer 1.0

NavigationPane {
    id: nav
    property variant tpage
    property int depth
    property variant spage
        
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
                        minHeight: 100
                        maxHeight: 100
                        minWidth: 100
                        maxWidth: 100
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
        
        actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
        
        Container {
            layout: DockLayout { }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            
            Container {
                
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                
                ActivityIndicator {
                    id: connectingActivity
                    preferredHeight: 60
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Top
                }
                Container {
                    background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#202020") : Color.create("#f5f5f5")
                    id: contactFilterGroup
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
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
                                contactFilterGroup.visible = false;
                                listContactView.requestFocus();
                            }
                        }
                        
                        input {
                            submitKey: SubmitKey.Send
                            onSubmitted: {
                                if(contactFilter.text != "")
                                    listContactsController.selectFirst();
                                 else {
                                     contactFilterGroup.visible = false;
                                     listContactView.requestFocus();
                                 }
                            }
                        }
                    }
                    ImageButton {
                        id: hideButton
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_top.png" : "asset:///images/icon_top_black.png"
                        onClicked: {
                            contactFilterGroup.visible = false;
                        }
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        visible: false
                    }
                    Container {
                        preferredWidth: 5
                    }
                    
                    onVisibleChanged: {
                        contactFilter.visible = visible;
                        hideButton.visible = visible;
                    }
                    visible: false;
                
                }
                ListView {
                    scrollRole: ScrollRole.Main
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
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
                        contactFilterGroup.visible = true;
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
                        sortingKeys: ["timestamp", "name"]
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
                                
                                ListItem.onSelectionChanged: {
                                    console.log('select')
                                    if(ListItem.selected)
                                        selectionIndicator.background = Color.create("#bd2d2d");
                                    else
                                        selectionIndicator.background = Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.Black : Color.White;
                                }
                                
                                layout: StackLayout {
                                    orientation: LayoutOrientation.TopToBottom
                                }
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Fill
                                
                                Container {
                                    minHeight: 10
                                    maxHeight: 10
                                }
                                
                                Container {
                                    preferredHeight: 140
                                    
                                    id: titleContainer
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Center
                                    
                                    Container {
                                        minWidth: 5
                                        maxWidth: 5
                                        id: selectionIndicator
                                        preferredHeight: 140
                                    }
                                    
                                    Container {
                                        layout: AbsoluteLayout { }
                                        minHeight: 140
                                        maxHeight: 140
                                        minWidth: 140
                                        maxWidth: 140
                                        
                                        //  Avatar
                                        ImageView {
                                            verticalAlignment: VerticalAlignment.Center
                                            //horizontalAlignment: HorizontalAlignment.Left
                                            id: avatarImg
                                            scalingMethod: ScalingMethod.AspectFit
                                            minHeight: 140
                                            maxHeight: 140
                                            minWidth: 140
                                            maxWidth: 140
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
                                            minHeight: 30
                                            maxHeight: 30
                                            minWidth: 30
                                            maxWidth: 30
                                            layoutProperties: AbsoluteLayoutProperties {
                                                positionX: 110
                                                positionY: 110
                                            }
                                            // 0 => online, 1 => away, 2 => away (long time), 3 => do not disturb, 4 => actively interested into chatting, 
                                            visible: ListItemData.presence == 0
                                        }
                                    
                                    }
                                    
                                    Container {
                                        minWidth: 40
                                        maxWidth: 40
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
                        //listContactsController.deleteHistory(id);
                        deleteToast.who = id;
                        deleteToast.dismissed = false;
                        deleteToast.show();
                        timerDelete.start();
                    }
                    
                    onTriggered: {
                        var chosenItem = dataModel.data(indexPath);
                        contactFilterGroup.visible = false;
                        
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
            
            Container {  
                id: dataEmptyLabel
                visible: theModel.empty //model.isEmpty() will not work  
                horizontalAlignment: HorizontalAlignment.Center  
                verticalAlignment: VerticalAlignment.Center  
                
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    ImageView {
                        imageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/pinch_out.png" : "asset:///images/pinch_out_black.png"
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        preferredHeight: 150
                        scalingMethod: ScalingMethod.AspectFit
                    }
                    
                    Label {
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        text: qsTr("Pinch out to stop filtering")  
                        textStyle.textAlign: TextAlign.Center  
                    }
                }
                
                Divider {
                
                }
                
                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    ImageView {
                        imageSource: "asset:///images/pull-to-refresh.png"
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        preferredHeight: 150
                        scalingMethod: ScalingMethod.AspectFit
                    }
                    
                    Label {
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        text: qsTr("Pull down to search a contact")  
                        textStyle.textAlign: TextAlign.Center  
                    }
                }
            }
        }
        
        onCreationCompleted: {
            
            listContactsController.setListView(listContactView);
            listContactsController.setActivityIndicator(connectingActivity);
            listContactsController.updateView();
            
            
            depth = 0;
        }
        
        actions: [
            ActionItem {
                title: qsTr("Change status")
                imageSource: "asset:///images/icon_quoted_white.png"
                onTriggered: {
                    if(!spage)
                        spage = statusSetter.createObject();
                    nav.push(spage);
                }
            },
            ActionItem {
                title: qsTr("Mark All as Read")
                imageSource: "asset:///images/icon_check.png"
                onTriggered: {
                    listContactsController.markAllRead();
                }
            },
            ActionItem {
                title: qsTr("Add contact")
                imageSource: "asset:///images/icon_add_contact.png"
                onTriggered: {
                    listContactsController.addContact();
                }
            },
            ActionItem {
                title: qsTr("Search contact")
                imageSource: "asset:///images/icon_search.png"
                onTriggered: {
                    contactFilterGroup.visible = true;
                    contactFilter.requestFocus();
                    
                }
                shortcuts: [
                    Shortcut {
                        key: "s"
                    }
                ]
            }
        ]
        
        attachedObjects: [
            ListContactsController {
                id: listContactsController
                
                onCleared: {
                    if(depth > 1)
                        nav.pop();
                
                }
                
                onUserSelected: {
                    // Create the content page and push it on top to drill down to it.
                    if(!tpage) {
                        tpage = conversation.createObject();
                    }
                    
                    // Set the url of the page to load and thread caption. 
                    tpage.name     = name;
                    tpage.avatar   = avatar;
                    tpage.id       = id;
                    tpage.room     = false;
                    
                    nav.push(tpage);
                }
                
            },
            ComponentDefinition {
                id: conversation
                source: "Conversation.qml"
            },
            ComponentDefinition {
                id: statusSetter
                source: "Status.qml"
            },
            SystemToast {
                id: deleteToast
                property string who
                property bool dismissed
                body: qsTr("History will be deleted") + Retranslate.onLanguageChanged
                button.label: qsTr("Undo")
                
                
                
                onFinished: {
                    console.log('undo' + value)
                    if(value == SystemUiResult.ButtonSelection){
                        dismissed = true;
                        // UNDO clicked - revert to normal Icon in this case
                    } else {
                        listContactsController.deleteHistory(who);
                    }
                }
            },
            QTimer {
                id: timerDelete
                singleShot: true
                interval: 3000
                
                onTimeout: {
                    if(!deleteToast.dismissed) {
                        deleteToast.cancel();
                        listContactsController.deleteHistory(deleteToast.who);
                    }
                }
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
            contactFilter.text = "";
            if(tpage) 
                tpage.id = "";
        
        }
        console.log(depth)
    
    }
    
}




