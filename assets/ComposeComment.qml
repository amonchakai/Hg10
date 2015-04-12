import bb.cascades 1.3

Page {
    property int number
    
    
    titleBar: TitleBar {
        title: qsTr("Comment")
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
                bugReportController.insertComment(description.text, parseInt(number));
            
            }
        }
    }
    
    Container {
        layout: StackLayout {}
        
        ActivityIndicator {
            id: postIssueActivity
        }
        
        TextArea {
            id: description
            hintText: qsTr("What is on your mind?")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }
    }
    
    onNumberChanged: {
        postIssueActivity.stop();
    }
}
