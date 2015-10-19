import bb.cascades 1.3
import Network.HistoryBrowserController 1.0

Page {
    property string id
    property string name
    property variant detailedHistoryPage
    
    titleBar: TitleBar {
        title: qsTr("Previous conversation")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                nav.pop();
            }
        }
    }
    
    Container {
        layout: DockLayout { }
        
        Label {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            
            text: qsTr("No history available.")
            visible: theModel.empty
        }
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            ActivityIndicator {
                id: activity
                preferredHeight: ui.du(6)
                horizontalAlignment: HorizontalAlignment.Fill
            }
            
            Container {
                background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#202020") : Color.create("#f5f5f5")
                id: searchGroup
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                TextField {
                    id: contactFilter
                    visible: false
                    hintText: qsTr("Keywords")
                    
                    input {
                        submitKey: SubmitKey.Send
                        onSubmitted: {
                            historyBrowserController.getThreads(id, name, contactFilter.text);
                            searchGroup.visible = false;
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
                id: listHistory
                focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                
                
                dataModel: GroupDataModel {
                    id: theModel
                    sortingKeys: ["id"]
                    grouping: ItemGrouping.None
                    sortedAscending: false
                    
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
                        type: "item"
                        
                        Container {
                            id: listItemContainer
                            
                            preferredHeight: ui.du(12)
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Center
                            layout: DockLayout {
                            }
                            
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                verticalAlignment: VerticalAlignment.Center
                                
                                Container {
                                    preferredWidth: ui.du(.2)
                                }
                                
                                Label {
                                    text: listItemContainer.ListItem.view.renderText(ListItemData.snippet)
                                    verticalAlignment: VerticalAlignment.Center
                                    horizontalAlignment: HorizontalAlignment.Left
                                }
                            }
                            
                            
                            Divider { }
                            
                            
                            
                            
                        }
                    }
                ]
                
                function renderText(text) {
                    return text.replace("&#39;", "\'").replace("&gt;", ">").replace("&lt;", "<");
                }
                
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    
                    if(!detailedHistoryPage)
                        detailedHistoryPage = detailedHistory.createObject();
                    detailedHistoryPage.id = chosenItem.id;

                    nav.push(detailedHistoryPage);
                
                }
            
            }
        }
    }
    
    actions: [
        ActionItem {
            id: search
            title: qsTr("Search")
            imageSource: "asset:///images/icon_search2.png"
            ActionBar.placement: ActionBarPlacement.Signature
            onTriggered: {
                searchGroup.visible = true;
                contactFilter.requestFocus();
            }
            
            shortcuts: [
                Shortcut {
                    key: "s"
                }
            ]
        }, 
        ActionItem {
            id: refresh
            title: qsTr("Refresh")
            imageSource: "asset:///images/icon_refresh.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                historyBrowserController.getThreads(id, name, "");
                activity.start();
            }
            
            shortcuts: [
                Shortcut {
                    key: "r"
                }
            ]
        }
    ]
    
    onCreationCompleted: {
        historyBrowserController.setListView(listHistory);
    }
    
    onIdChanged: {
        historyBrowserController.getThreads(id, name, "");
        activity.start();
    }
    
    attachedObjects: [
        HistoryBrowserController {
            id: historyBrowserController
            
            onCompleted: {
                activity.stop();
                listHistory.requestFocus();
            }
            
        }, 
        ComponentDefinition {
            id: detailedHistory
            source: "DetailedHistory.qml"
        }
    ]
    
}
