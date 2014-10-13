import bb.cascades 1.2
import Network.ListContactsController 1.0
import com.netimage 1.0

Page {
    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                leftPadding: 10
                rightPadding: 10
                
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
                        color: Color.Black
                    }
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties { spaceQuota: 1 }
                }
            }
        }
    }
    
    Container {
        ListView {
            id: listContactView
            dataModel: GroupDataModel {
                id: theModel
                sortingKeys: ["category"]
                
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
                            
                            Container {
                                minWidth: 30
                                maxWidth: 30
                                //horizontalAlignment: HorizontalAlignment.Left
                                
                            }
                            
                            Container {
                                id: contactContainer
                                preferredWidth: 1000
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
                                        text: ListItemData.timestamp
                                        horizontalAlignment: HorizontalAlignment.Right
                                        verticalAlignment: VerticalAlignment.Center
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SmallText
                                            color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#00a8df") : Color.Blue
                                        }
                                    }
                                }
                                
                                
                                Container {
                                    layout: DockLayout {
                                    }
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Bottom
                                    
                                    
                                    Label {
                                        text: "hello this is some previous message"
                                        horizontalAlignment: HorizontalAlignment.Left
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SmallText
                                            color: Color.Gray
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
            
        }
    }
    
    onCreationCompleted: {
        listContactsController.setListView(listContactView);
    }
    
    attachedObjects: [
        ListContactsController {
            id: listContactsController
        }
    ]
}
