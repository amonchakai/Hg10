import bb.cascades 1.2
import Network.SmileyPickerController 1.0
import com.netimage 1.0

Page {
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }

        ListView {
            id: smileyList
            horizontalAlignment: HorizontalAlignment.Center
            
            layout: GridListLayout {
                columnCount: 4
            }
            
            dataModel: GroupDataModel {
                id: theModel
                grouping: ItemGrouping.None
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        
                        ImageView {
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
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
                                    
                                    source: ListItemData.localUrl                                    
                                } 
                            ]
                        }
                        
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                smileyToAdd = chosenItem.distUrl;
                nav.pop();
            }
        }    
    }
    
    attachedObjects: [
    	SmileyPickerController {
            id: smileyPickerController

        }    
    ]
    
    onCreationCompleted: {
        smileyPickerController.setListView(smileyList);
        smileyPickerController.loadDefautSmiley();
        
    }
}
