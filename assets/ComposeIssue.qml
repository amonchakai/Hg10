import bb.cascades 1.3

Page {
    property int label
    
    
    titleBar: TitleBar {
        title: qsTr("Compose")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                navBug.pop();
            }
        }
        acceptAction: ActionItem {
            title: qsTr("Post")
            onTriggered: {
                bugReportController.insertIssue(caption.text, description.text, label);
                
            }
        }
    }
    
    Container {
        layout: StackLayout {}
        
        ActivityIndicator {
            id: postIssueActivity
        }
        
        TextField {
            id: caption
            hintText: qsTr("Title")
        }
        
        TextArea {
            id: description
            hintText: qsTr("Description")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }
    }
    
    onLabelChanged: {
        postIssueActivity.stop();
    }
    
}
