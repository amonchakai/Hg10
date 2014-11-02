import bb.cascades 1.2
import Network.SuggestContactController 1.0
import com.netimage 1.0

Page {
    titleBar: TitleBar {
        title: qsTr("Create room")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                nav.pop();
            }
        }
        acceptAction: ActionItem {
            title: qsTr("Create")
            onTriggered: {
                nav.newParticipants = participants.text;
                
                nav.pop();
                
            }
        }
    }
    
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
                        
        Label {
            text: qsTr("Participants")
        }
        TextField {
            id: participants
            accessibility.name: "participants"
            hintText: qsTr("Look for a contact")
            
            onTextChanging: {
                if(text.length > 3) {
                    suggestContact.suggest(text);
                }
            }
        }
        
        Divider { }
        
        ListView {
            id: suggestionsList
            
            dataModel: GroupDataModel {
                id: theModel
                grouping: ItemGrouping.None
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
                    
                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            preferredHeight: 110
                            
                            ImageView {
                                verticalAlignment: VerticalAlignment.Center
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
                            
                            Label {
                                id: name
                                text: ListItemData.name
                                verticalAlignment: VerticalAlignment.Center
                            }
                        }
                        
                        Divider {
                            
                        }
                    }
                }
                
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                
                participants.text = suggestContact.addParticipant(participants.text, chosenItem.name);
            }
        }
        
    }
    
    onCreationCompleted: {
        suggestContact.setListView(suggestionsList);
    }
    
    attachedObjects: [
        SuggestContactController {
            id: suggestContact
        }
    ]
    
}

