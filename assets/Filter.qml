import bb.cascades 1.3

Page {
    titleBar: TitleBar {
        title: qsTr("Filter contacts")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                nav.pop();
            }
        }
        acceptAction: ActionItem {
            title: qsTr("Set")
            onTriggered: {
                listContactsController.setFilter(onlyFavToggle.checked);
                
                var code = 0;
                if(onlyConnectedToggle.checked)
                    code = 10;
                if(onlyAvailableToggle.checked)
                    code += 1;
                
                if(code == 0)
                    listContactsController.setAvailabilityFilter(-1);
                 else 
                     listContactsController.setAvailabilityFilter(code);
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
        
        
        Container {
            layout: DockLayout {
                
            }
            horizontalAlignment: HorizontalAlignment.Fill
            
            Label {
                text: qsTr("Show only favorite")
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
                
            }
            
            ToggleButton {
                id: onlyFavToggle
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                
                checked: listContactsController.showOnlyFav
                
            }
            
            leftMargin: ui.du(1)
            rightMargin: ui.du(1)
        }
        
        Divider {  }
        
        Container {
            layout: DockLayout {
            
            }
            horizontalAlignment: HorizontalAlignment.Fill
            
            Label {
                text: qsTr("Show only available contacts")
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            
            ToggleButton {
                id: onlyAvailableToggle
                
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                
                checked: listContactsController.availabilityFilter % 2 && listContactsController.availabilityFilter >= 0
            
            }
            
            leftMargin: ui.du(1)
            rightMargin: ui.du(1)
        }
        
        Divider {  }
        
        Container {
            layout: DockLayout {
            
            }
            horizontalAlignment: HorizontalAlignment.Fill
            
            Label {
                text: qsTr("Show only connected contacts")
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            
            }
            
            ToggleButton {
                id: onlyConnectedToggle
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                
                checked: listContactsController.availabilityFilter > 9
            
            }
            
            leftMargin: ui.du(1)
            rightMargin: ui.du(1)
        }
        
        Divider {  }
        
    }
}
