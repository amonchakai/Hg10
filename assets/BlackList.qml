import bb.cascades 1.3

Page {
    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: DockLayout { }
                leftPadding: 10
                rightPadding: 10
                
                Label {
                    text: qsTr("Blacklist")
                    textStyle {
                        color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.White : Color.Black
                    }
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
        }
    }
    
    Container {
        layout: DockLayout {
            
        }
        
        Container {  
            id: dataEmptyLabel
            visible: theModel.empty //model.isEmpty() will not work  
            horizontalAlignment: HorizontalAlignment.Center  
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout {}
            
            Label {
                text: qsTr("Your blacklist is empty.")
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                textStyle.textAlign: TextAlign.Center
                multiline: true
            }
        }
        
        ListView {
            id: blacklistView
            
            focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
            
            dataModel: ArrayDataModel {
                id: theModel
                
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
            
            listItemComponents: [
                ListItemComponent {
                    type: ""
                    
                    CustomListItem {
                        id: customListItem
                        
                        Container {
                            preferredHeight: ui.du(12)
                            id: listItemContainer
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Center
                            layout: DockLayout {
                            }
                            
                            Container {
                                verticalAlignment: VerticalAlignment.Center
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                Container {
                                    preferredWidth: ui.du(0.1)
                                }
                                Label {
                                    text: ListItemData
                                    verticalAlignment: VerticalAlignment.Center
                                }
                            }
                        
                        }
                        
                        contextActions: [
                            ActionSet {
                                title: qsTr("Blacklist")
                                
                                DeleteActionItem {
                                    title: qsTr("Remove from blacklist")
                                    onTriggered: {
                                        customListItem.ListItem.view.removeFromBlacklist(ListItemData);
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
            
            function removeFromBlacklist(id) {
                listContactsController.removeFromBlackList(id);
            }
            
        }
    }
    
    onCreationCompleted: {
        listContactsController.setBlackListView(blacklistView);
        listContactsController.updateBlackList();
        blacklistView.requestFocus();
    }
}
