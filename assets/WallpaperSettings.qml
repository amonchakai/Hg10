import bb.cascades 1.3
import Network.CustomizationController 1.0
import Lib.QTimer 1.0
import bb.system 1.2

Page {
    
    property variant editorPage
    
    titleBar: TitleBar {
        title: qsTr("Wallpapers")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                navSettings.pop();
            }
        }
    }
    
    Container {
        layout: DockLayout { }
    
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
    
            ListView {
                id: listCustomization
                dataModel: GroupDataModel {
                    id: theModel
                    sortingKeys: ["userName"]
                    sortedAscending: false
                    grouping: ItemGrouping.None
                    
                    property bool empty: true
                    
                    
                    onItemAdded: {
                        listCustomization.scroll(-1);
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
                            
                            Container {
                                preferredHeight: ui.du(1)
                            }
                            
                            Container {
                                preferredWidth: 2000
                                
                                
                                layout: DockLayout { }
                                
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    verticalAlignment: VerticalAlignment.Center
                                    horizontalAlignment: HorizontalAlignment.Left
                                    
                                    ImageView {
                                        horizontalAlignment: HorizontalAlignment.Right
                                        verticalAlignment: VerticalAlignment.Center
                                        imageSource: ListItemData.wallpaper
                                        preferredHeight: ui.du(10)
                                        preferredWidth: ui.du(10)
                                        margin.leftOffset: ui.du(1)
                                    }
                                    
                                    
                                    Label {
                                        text: ListItemData.userName
                                        horizontalAlignment: HorizontalAlignment.Left
                                        verticalAlignment: VerticalAlignment.Center
                                    }
                                }
                                
                                    
                                 ImageView {
                                     
                                     imageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_right.png" : "asset:///images/icon_right_black.png"
                                     preferredHeight: ui.du(5)
                                     preferredWidth: ui.du(5)
                                     verticalAlignment: VerticalAlignment.Center
                                     horizontalAlignment: HorizontalAlignment.Right
                                     margin.rightOffset: ui.du(1)
                                 }
                                
                            }
                            
                            Divider { }
                            
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Management")
                                    
                                    DeleteActionItem {
                                        title: qsTr("Delete")
                                        onTriggered: {
                                            overallContactContainer.ListItem.view.deleteSettings(ListItemData.userId, overallContactContainer.ListItem.indexPath);
                                        }
                                    }
                                }
                            ]
                                                    
                        }
                    }
                ]
                
                function deleteSettings(id, path) {
                    deleteToast.who = id;
                    deleteToast.path = path;
                    
                    deleteToast.dismissed = false;
                    deleteToast.show();
                    timerDelete.start();
                }
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    
                    if(!editorPage)
                        editorPage = themeEditor.createObject();
                    
                    editorPage.userId = chosenItem.userId;
                    
                    navSettings.push(editorPage);
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
            
            Label {
                text: qsTr("You can use the Background option in an opened converation to define a per-user wallpaper")
                horizontalAlignment: HorizontalAlignment.Center
                textStyle.textAlign: TextAlign.Center
                multiline: true
            }
            
            ImageView {
                imageSource: "asset:///images/icon_wallpaper.png"
                preferredHeight: ui.du(10)
                preferredWidth: ui.du(10)
                horizontalAlignment: HorizontalAlignment.Center
            }
            
            
            
        }
    }
    
    actions: [
        ActionItem {
            title: qsTr("Create")
            ActionBar.placement: ActionBarPlacement.Signature
            imageSource: "asset:///images/icon_add.png"
            
            onTriggered: {
                customController.createDefault();
            }
        }
    ]
    
    onCreationCompleted: {
        customController.setListView(listCustomization);
        customController.updateView();
    }
    
    attachedObjects: [
        CustomizationController {
            id: customController
        },
        SystemToast {
            id: deleteToast
            property string who
            property variant path
            property bool dismissed
            body: qsTr("Settings will be deleted") + Retranslate.onLanguageChanged
            button.label: qsTr("Undo")
            
            
            
            onFinished: {
                console.log('undo' + value)
                if(value == SystemUiResult.ButtonSelection){
                    dismissed = true;
                    // UNDO clicked - revert to normal Icon in this case
                } else {
                    listCustomization.dataModel.removeAt(path);
                    customController.deleteCustom(who);
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
                    listCustomization.dataModel.removeAt(deleteToast.path);
                    customController.deleteCustom(deleteToast.who);
                }
            }
        },
        ComponentDefinition {
            id: themeEditor
            source: "ThemeEditor.qml"
        }
    ]
}
