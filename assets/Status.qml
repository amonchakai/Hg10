import bb.cascades 1.2

Page {
    titleBar: TitleBar {
        title: qsTr("Set your status")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                nav.pop();
            }
        }
        acceptAction: ActionItem {
            title: qsTr("Set")
            onTriggered: {
                listContactsController.setPresence(statusText.text, statusDropdown.selectedValue);
                nav.pop();
            }
        }
    }
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        DropDown {
            id: statusDropdown
            title: qsTr("Status")
            
            options: [
                Option {
                    text: qsTr("Available")
                    value: 0
                },
                Option {
                    text: qsTr("Busy")
                    value: 3
                }
            ]
            
            selectedIndex: listContactsController.available == 0 ? 0 : 1
        }
        
        TextField {
            id: statusText
            hintText: qsTr("What is your mood?")
            text: listContactsController.presence
        }
        
    }
}
