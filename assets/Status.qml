import bb.cascades 1.3

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
                listContactsController.setPresence(statusText.text, statusDropdown.selectedValue, setPriority.checked, priority.text);
                nav.pop();
            }
        }
    }
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        // --------------------------------------------------------------------------
        Container {
            preferredHeight: ui.du(4)
        }
        
        Container {
            layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
            
            Label {
                text: qsTr("Status")
                textStyle.fontSize: FontSize.Large
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Bottom
            }
        
        }
        Divider { }
        
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
        
        
        
        
        // --------------------------------------------------------------------------
        Container {
            preferredHeight: ui.du(4)
        }
        
        Container {
            layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
            
            Label {
                text: qsTr("Advanced settings")
                textStyle.fontSize: FontSize.Large
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Bottom
            }
        
        }
        Divider { }
        
        
        Container {
            layout: DockLayout { }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                text: qsTr("Set XMPP Priority:")
            }
            
            ToggleButton {
                id: setPriority
                checked: listContactsController.choosePriority
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
            }

        }
        TextField {
            id: priority
            hintText: qsTr("(default 20)")
            text: listContactsController.priority
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Fill
        }
    }
    
    onCreationCompleted: {
        console.log('priority: ' + listContactsController.priority)
    }
}
