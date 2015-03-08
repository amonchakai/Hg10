import bb.cascades 1.3

Page {
    property string id
    property string fullname
    
    
    titleBar: TitleBar {
        title: qsTr("Edit contact")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                nav.pop();
            }
        }
        acceptAction: ActionItem {
            title: qsTr("Set")
            onTriggered: {
                
                listContactsController.editContact(id, setFullName.text);
                
                nav.pop();            
            }
        }
    }
    
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        Container {
            preferredHeight: ui.du(3)
            preferredWidth: ui.du(3)
        }
        
        Label {
            text: qsTr("Full name:")
        }
        TextField {
            id: setFullName
            hintText: qsTr("Set user name")
            text: fullname
        }
        
        Divider { }
        
        
    }
}
