import bb.cascades 1.3

Page {
    property string id
    property string fullname
    property string nickname
    property string firstname
    property string lastname
    
    
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
        
        
        Label {
            text: qsTr("Nickname:")
        }
        TextField {
            id: setNickname
            hintText: qsTr("Set nickname")
            text: nickname
        }
        
        Divider { }
        
        Label {
            text: qsTr("First name:")
        }
        TextField {
            id: setFirstname
            hintText: qsTr("Set first name")
            text: firstname
        }
        
        Divider { }
        
        
        Label {
            text: qsTr("Last name:")
        }
        TextField {
            id: setLastname
            hintText: qsTr("Set last name")
            text: lastname
        }
        
        Divider { }
        
    }
    
    onFirstnameChanged: {
        setFirstname.text = firstname;
    }
    onLastnameChanged: {
        setLastname.text = lastname;
    }
    onNicknameChanged: {
        setNickname.text = nickname;
    }
}
