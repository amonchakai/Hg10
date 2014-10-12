import bb.cascades 1.2
import Network.ListContactsController 1.0
import com.netimage 1.0

Page {
    Container {
        Button {
            text: "update"
            onClicked: {
                listContactsController.updateView();
            }
        }
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
                        function colorIndexToHex(index) {
                            switch (index) {
                                case 0:
                                    return Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "#000000" : "#ffffff" ;
                                
                                case 1:
                                    return "#a8cc0a";
                                
                                case 2:
                                    return "#27bbc6";
                                
                                case 3:
                                    return "#fe5e05";
                                
                                case 4:
                                    return "#ef0061";
                                
                                case 5:
                                    return "#795ae0";
                                
                                default:
                                    return "#ffffff";
                            }
                        }
                        
                        id: titleContainer
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                        
                        // TODO Avatar
                        ImageView {
                            id: avatarImg
                            scalingMethod: ScalingMethod.AspectFit
                            minHeight: 80
                            maxHeight: 80
                            image: tracker.image
                            
                            attachedObjects: [
                                NetImageTracker {
                                    id: tracker
                                    
                                    source: ListItemData.avatar                                    
                                } 
                            ]
                        
                        }
                        
                        Container {
                            id: contactContainer
                            
                            layout: StackLayout {
                                orientation: LayoutOrientation.TopToBottom
                            }
                            verticalAlignment: VerticalAlignment.Top
                            Label {
                                text: ListItemData.name
                            }
                            
                            Container {
                                layout: DockLayout {
                                }
                                horizontalAlignment: HorizontalAlignment.Fill
                                
                                Label {
                                    text: ListItemData.timestamp
                                    horizontalAlignment: HorizontalAlignment.Right
                                    textStyle {
                                        base: SystemDefaults.TextStyles.SmallText
                                        color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#00a8df") : Color.Blue
                                    }
                                }
                                
                                Label {
                                    text: ListItemData.preview
                                    horizontalAlignment: HorizontalAlignment.Left
                                    textStyle {
                                        base: SystemDefaults.TextStyles.SmallText
                                        color: Color.Gray
                                    }
                                }
                            }
                            Divider {}
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
