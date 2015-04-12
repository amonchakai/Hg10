import bb.cascades 1.3
import Lib.BugReport 1.0


Page {
    
    property int typeIssue
    property string category
    property string catImg
    
    property variant issuePage
    property variant createIssuePage
    
    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: DockLayout { }
                leftPadding: 10
                rightPadding: 10
                background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#282828") : Color.create("#f0f0f0");
                
                ImageButton {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                    defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_left.png" : "asset:///images/icon_left_black.png"
                    onClicked: {
                        navBug.pop();
                    }
                }  
                
                Label {
                    id: folder
                    text: category
                    textStyle {
                        color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.White : Color.Black
                    }
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
                
                ImageView {
                    imageSource: catImg
                    preferredHeight: ui.du(7)
                    scalingMethod: ScalingMethod.AspectFit
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                }
                
                
            }
        }
    }
    
    id: pageListIssues
    
    function isPassort() {
        return DisplayInfo.width == 1440 && DisplayInfo.height == 1440;
    }
    
    Container {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        layout: DockLayout { }
        
        Container {  
            id: dataEmptyLabel
            visible: theModel.empty //model.isEmpty() will not work  
            horizontalAlignment: HorizontalAlignment.Center  
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout {}
            
            Label {
                text: connectingActivity.running ?  qsTr("Loading, please wait.") : qsTr("No issues in this category.")
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            ActivityIndicator {
                id: connectingActivity
                preferredHeight: 60
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top
            }
            
            ListView {
                id: currentIssuesList
                
                dataModel: GroupDataModel {
                    id: theModel
                    sortingKeys: ["number"]
                    sortedAscending: false
                    grouping: ItemGrouping.None
                    
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
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Center
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            preferredHeight: listItemContainer.ListItem.view.containerHeight();
                             
                            Container {
                                preferredWidth: ui.du(0.6)
                            }
                             
                             
                            Container {
                                layout: DockLayout {}
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Center
                                preferredHeight: listItemContainer.ListItem.view.containerSpacing();
                                 
                                Label {
                                    text: ListItemData.title
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Top
                                }
                                 
                                Container {
                                     layout: DockLayout { }
                                     horizontalAlignment: HorizontalAlignment.Fill
                                     verticalAlignment: VerticalAlignment.Bottom
                                     
                                     Label {
                                         text: ListItemData.author
                                         textStyle.fontSize: FontSize.XSmall
                                         horizontalAlignment: HorizontalAlignment.Left
                                         verticalAlignment: VerticalAlignment.Center
                                     }
                                     
                                     Container {
                                         horizontalAlignment: HorizontalAlignment.Right
                                         verticalAlignment: VerticalAlignment.Center
                                         
                                         layout: DockLayout {}
                                         
                                         ImageView {
                                             imageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_quoted_white.png" : "asset:///images/icon_quoted.png"
                                             minHeight: ui.du(3)
                                             minWidth: ui.du(3)
                                             visible: ListItemData.comments != 0
                                             horizontalAlignment: HorizontalAlignment.Center
                                             verticalAlignment: VerticalAlignment.Center
                                         }
                                         
                                         Label {
                                             text: ListItemData.comments
                                             textStyle.fontSize: FontSize.XXSmall
                                             visible: ListItemData.comments != 0
                                             textStyle.color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.Black : Color.White
                                             horizontalAlignment: HorizontalAlignment.Center
                                             verticalAlignment: VerticalAlignment.Top
                                             
                                         }
                                     
                                     }
                                 }
                                Divider { }
                                 
                             }
                         }
                     }
                ]
                
                function containerHeight() {
                    return pageListIssues.isPassort() ? ui.du(14) : ui.du(11);
                }
                
                function containerSpacing() {
                    return pageListIssues.isPassort() ? ui.du(11) : ui.du(9);
                }
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    
                    if(!issuePage)
                        issuePage = issueViewer.createObject();
                    
                    issuePage.caption = chosenItem.title;
                    issuePage.number = chosenItem.number;
                    navBug.push(issuePage);                                    
                
                }
            }
            
        }
    }
    
    actions: [
        
        ActionItem {
            title: qsTr("New")
            ActionBar.placement: ActionBarPlacement.Signature
            imageSource: "asset:///images/icon_pen.png"
            
            onTriggered: {
                if(!createIssuePage)
                    createIssuePage = composeIssue.createObject();
                
                createIssuePage.label = typeIssue;
                navBug.push(createIssuePage);
            }
        }
        
    ]
    
    onCreationCompleted: {
        bugReportController.setListView(currentIssuesList);
    }
    
    onTypeIssueChanged: {
        bugReportController.listIssues(typeIssue);
        connectingActivity.start();
    }
    
    attachedObjects: [
        BugReport {
            id: bugReportController
            
            onCompleted: {
                connectingActivity.stop();
            }
            
            onInsertSuccess: {
                navBug.pop();
                connectingActivity.start();
            }
            
            onInsertCommentSuccess: {
                navBug.pop();
            }
        },
        ComponentDefinition {
            id: issueViewer
            source: "IssueViewer.qml"
        },
        ComponentDefinition {
            id: composeIssue
            source: "ComposeIssue.qml"
        }
    ]
}
