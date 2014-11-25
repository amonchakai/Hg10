import bb.cascades 1.2

Page {    
    titleBar: TitleBar {
        title: qsTr("Help")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                furtherInformation.close();
            }
        }
    }
    
    ScrollView {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            Label {
                text: qsTr("Problem:")
                textStyle.fontWeight: FontWeight.Bold
            }
            Label {
                multiline: true
                text: qsTr("Apparently you are having some difficulties to connect to your account. If you are trying to connect to Google, it may be Google which for safety reason is blocking the application to access to your account. Indeed, it only expect an official hangout client or Gmail.")
            }
            
            Divider { }
            
            Label {
                text: qsTr("Solution:")
                textStyle.fontWeight: FontWeight.Bold
            }
            Label {
                multiline: true
                text: qsTr("Google have a documentation on how to help you to use this application (and other application) when you are having difficulites.")
            }
            Button {
                text: qsTr("Open Google\'s manual")
                horizontalAlignment: HorizontalAlignment.Center
                onClicked: {
                    linkInvocation.query.uri = "https://support.google.com/mail/answer/14257?hl=en&rd=1";
                }
            }
            
            Divider { }
            
            Label {
                text: qsTr("Still stuck?")
                textStyle.fontWeight: FontWeight.Bold
            }
            Label {
                multiline: true
                text: qsTr("You can have different ways to contact me: either on Github, or by email. Please check the contact information on the BlackBerry© World page.")
            }
            
        }
    }
    
    attachedObjects: [
        Invocation {
            id: linkInvocation
            
            query.invokeTargetId: "sys.browser";
            query.invokeActionId: "bb.action.OPEN"
            
            
            query {
                onUriChanged: {
                    linkInvocation.query.updateQuery();
                }
            }
            
            onArmed: {
                
                trigger("bb.action.OPEN");
            }
        }
    ]
}
