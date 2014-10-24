import bb.cascades 1.2
import Network.LoginController 1.0
import bb.cascades 1.2
import Network.ListContactsController 1.0
import com.netimage 1.0

NavigationPane {
    id: nav
    property variant tpage
    property int depth
    
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: qsTr("About")
                imageSource: "asset:///images/icon_about.png"
                onTriggered: {
                }
            },
            ActionItem {
                title: qsTr("Settings")
                imageSource: "asset:///images/icon_settings.png"
                onTriggered: {
                    settings.open();
                }
            }
        ]
    }  
    
    
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
                        minHeight: 90
                        maxHeight: 90
                        minWidth: 90
                        maxWidth: 90
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
            /*Container {
                minHeight: 4
                maxHeight: 4
                background: Color.create("#0677a3")
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top
            }*/
            ActivityIndicator {
                id: connectingActivity
                preferredHeight: 60
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top
            }
            ListView {
                id: listContactView
                dataModel: GroupDataModel {
                    id: theModel
                    sortingKeys: ["timestamp"]
                    sortedAscending: false
                    grouping: ItemGrouping.None
                    
                    property bool empty: true
                    
                    
                    onItemAdded: empty = isEmpty()  
                    onItemRemoved: empty = isEmpty()  
                    onItemUpdated: empty = isEmpty()  
                    
                    // You might see an 'unknown signal' error  
                    // in the QML-editor, guess it's a SDK bug.  
                    onItemsChanged: empty = isEmpty()
                
                }
                
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
                            
                            Divider {
                            
                            }
                        
                        }
                    }
                ]
                
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
                    
                    nav.push(tpage);
                }
            
            }
        }
        
        onCreationCompleted: {
            if(!loginController.isLogged()) {
                welcome.open();
            }
            
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
            },
            Sheet {
                id: welcome
                Welcome {
                    onDone: {
                        welcome.close();
                    }
                }
            },
            Sheet {
                id: settings
                Settings {
                    onDone: {
                        settings.close();
                    }
                }
            },
            LoginController {
                id: loginController
            }
        ]
    
    }
    
    onPushTransitionEnded: {
        ++depth;
        console.log(depth)
    }
    
    onPopTransitionEnded: {
        --depth;
        if(depth == 1) {
            listContactsController.markRead();
            if(tpage) 
                tpage.id = "";
        
        }
        console.log(depth)
    }
}


/*
 * Tabs may not really be needed... 
 * 



TabbedPane {
    id: mainTab
    showTabsOnActionBar: false
    activeTab: tabHome
        
    attachedObjects: [
        
        Sheet {
            id: welcome
            Welcome {
                onDone: {
                    welcome.close();
                }
            }
        },
        Sheet {
            id: settings
            Settings {
                onDone: {
                    settings.close();
                }
            }
        },
        
        LoginController {
            id: loginController
            
         }
    ]
    
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: qsTr("About")
                imageSource: "asset:///images/icon_about.png"
                onTriggered: {
                }
            },
            ActionItem {
                title: qsTr("Settings")
                imageSource: "asset:///images/icon_settings.png"
                onTriggered: {
                    settings.open();
                }
            }
        ]
    }    

    Tab { //First tab
        // Localized text with the dynamic translation and locale updates support
        id: tabHome
        title: qsTr("Home") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/icon_home.png"

        delegateActivationPolicy: TabDelegateActivationPolicy.Default

        delegate: Delegate {
            source: "Home.qml"
        }

    } //End of home tab


    Tab { //Favorite tab
        id: tabFav
        title: qsTr("Favorite") + Retranslate.onLocaleOrLanguageChanged
        
        imageSource: "asset:///images/icon_favorites.png"
        
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateImmediately
        
        delegate: Delegate {
            source: "Favorite.qml"
        }
    
    } //End of favorite tab
    
    onCreationCompleted: {
        if(!loginController.isLogged()) {
            welcome.open();
        }
    }
    
    onActiveTabChanged: {
        
    }
}

*/



