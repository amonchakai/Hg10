import bb.cascades 1.3

Page {
    property int number
    property string caption
    property variant composeCommentPage
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        TextArea {
            text: caption
            enabled: false;
        }
        
        ScrollView {
            WebView {
                id: issueWebView
                focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                
                settings.textAutosizingEnabled: false
                settings.zoomToFitEnabled: false
                
                onLoadingChanged: {
                    if (loadRequest.status == WebLoadStatus.Succeeded) {
                        issueWebView.requestFocus();
                    }
                }
            }            
        }
        
    }
    
    actions: [
        ActionItem {
            title: qsTr("New")
            ActionBar.placement: ActionBarPlacement.Signature
            imageSource: "asset:///images/icon_pen.png"
            
            onTriggered: {
                if(!composeCommentPage)
                    composeCommentPage = composeComment.createObject();
                
                composeCommentPage.number = number;
                navBug.push(composeCommentPage);
            }
        }
    ]
    
    onNumberChanged: {
        bugReportController.loadIssue(number);
    }
    
    onCreationCompleted: {
        bugReportController.setWebView(issueWebView);
    }
    
    attachedObjects: [
        
        ComponentDefinition {
            id: composeComment
            source: "ComposeComment.qml"
        }
    ]
}
