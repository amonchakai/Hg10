import bb.cascades 1.2
import Network.DriveController 1.0
import com.netimage 1.0
    
Page {
 titleBar: TitleBar {
     kind: TitleBarKind.FreeForm
     kindProperties: FreeFormTitleBarKindProperties {
         Container {
             layout: DockLayout { }
             leftPadding: 10
             rightPadding: 10
             background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#282828") : Color.create("#f0f0f0");
             
             Label {
                 id: folder
                 text: "My Drive"
                 textStyle {
                     color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.White : Color.Black
                 }
                 verticalAlignment: VerticalAlignment.Center
                 horizontalAlignment: HorizontalAlignment.Center
             }
             
             ImageButton {
                 verticalAlignment: VerticalAlignment.Center
                 horizontalAlignment: HorizontalAlignment.Left
                 defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/icon_left.png" : "asset:///images/icon_left_black.png"
                 onClicked: {
                     driveController.pop();
                 }
             }            
         }
     }
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
             text: connectingActivity.running ?  qsTr("Loading, please wait.") : qsTr("No files found in this folder.")
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
             id: fileListView
             dataModel: GroupDataModel {
                 id: theModel
                 sortingKeys: ["type"]
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
                         verticalAlignment: VerticalAlignment.Center
                         preferredHeight: 100
                         
                         Container {
                             preferredHeight: 50
                         }
                         
                         Container {
                             layout: StackLayout {
                                 orientation: LayoutOrientation.LeftToRight
                             }
                             horizontalAlignment: HorizontalAlignment.Fill
                             verticalAlignment: VerticalAlignment.Center
                             
                             Container {
                                 preferredWidth: 15
                                 preferredHeight: 100
                             }
                             
                             ImageView {
                                 verticalAlignment: VerticalAlignment.Center
                                 //horizontalAlignment: HorizontalAlignment.Left
                                 id: avatarImg
                                 scalingMethod: ScalingMethod.AspectFit
                                 minHeight: 40
                                 maxHeight: 40
                                 minWidth: 40
                                 maxWidth: 40
                                 image: tracker.image
                                 
                                 attachedObjects: [
                                     NetImageTracker {
                                         id: tracker
                                         
                                         source: ListItemData.iconLink                                    
                                     } 
                                 ]
                             }
                             
                             Container {
                                 preferredHeight: 15
                                 preferredWidth: 15
                             }
                             
                             Container {
                                 layout: DockLayout {
                                 }
                                 horizontalAlignment: HorizontalAlignment.Fill
                                 verticalAlignment: VerticalAlignment.Center
                                 preferredHeight: 100
                                 
                                 Label {
                                     text: ListItemData.title
                                     horizontalAlignment: HorizontalAlignment.Left
                                     verticalAlignment: VerticalAlignment.Top
                                 }
                                 
                                 Label {
                                     text: ListItemData.timestamp
                                     horizontalAlignment: HorizontalAlignment.Left
                                     verticalAlignment: VerticalAlignment.Bottom
                                     textStyle {
                                         base: SystemDefaults.TextStyles.SmallText
                                     }
                                 }
                             }
                         
                         }
                         
                         Divider { }
                         
                         contextActions: [
                             ActionSet {
                                 title: qsTr("Actions")
                                 
                                 ActionItem {
                                     title: qsTr("Set as home folder")
                                     imageSource: "asset:///images/icon_home.png"
                                     onTriggered: {
                                         overallContactContainer.ListItem.view.setHome(ListItemData.id);
                                     }
                                 }
                             }
                         ]
                     }
                 }
             ]
             
             function setHome(id) {
                 driveController.setHomeFolder(id);
             }
             
             onTriggered: {
                 var chosenItem = dataModel.data(indexPath);
                 
                 driveController.openForSharing(chosenItem.id, chosenItem.type);
                 
                 if(chosenItem.type == "application/vnd.google-apps.folder")
                     connectingActivity.start();
             
             
             }
         
         
         
         }
     
     }
 
 }
 
 actions: [
     ActionItem {
         id: refresh
         title: qsTr("Refresh")
         imageSource: "asset:///images/icon_refresh.png"
         onTriggered: {
             driveController.updateView();
         }
     },
     ActionItem {
         id: createFolder
         title: qsTr("Create a new folder")
         imageSource: "asset:///images/icon_folder.png"
         onTriggered: {
             driveController.createNewFolder();
         }
     }
 ]
 
 
 onCreationCompleted: {
     driveController.setListView(fileListView);
     driveController.getFileList();
     connectingActivity.start();
 }
 
 attachedObjects: [
     DriveController {
         id: driveController
         
         onComplete: {
             connectingActivity.stop();
         }
         
     },
     ComponentDefinition {
         id: webEdior
         source: "WebEditor.qml"
     }
 ]
}
