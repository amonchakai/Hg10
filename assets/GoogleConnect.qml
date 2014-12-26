import bb.cascades 1.2
import Network.GoogleConnectController 1.0

Page {    
    property bool checkPermissions
    
    titleBar: TitleBar {
        title: qsTr("Google")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                navSettings.pop();
            }
        }
    }
    
    Container {
        layout: DockLayout {
            
        }
        
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            id: scrollView
            
            Container {
                id: permissionsContainer
                
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                verticalAlignment: VerticalAlignment.Center
                
                Label {
                    visible: checkPermissions
                    text: qsTr("Permissions")
                }
                
                Divider { visible: checkPermissions }
                
                Container {
                    visible: checkPermissions
                    preferredHeight: 20
                }
                
                Container {
                    visible: checkPermissions
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    CheckBox {
                        visible: checkPermissions
                        checked: true
                        enabled: false
                        verticalAlignment: VerticalAlignment.Center
                    }
                    Label {
                        visible: checkPermissions
                        text: qsTr("Instant messaging service: Connect to Google Hangout")
                        multiline: true
                    }
                }
                
                Container {
                    visible: checkPermissions
                    preferredHeight: 20
                }
                
                Divider { visible: checkPermissions }
                
                Container {
                    visible: checkPermissions
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    CheckBox {
                        id: gmailAccess
                        visible: checkPermissions
                        checked: true
                        verticalAlignment: VerticalAlignment.Center
                    }
                    Label {
                        visible: checkPermissions
                        text: qsTr("Gmail: Retrieve history, enabling synchronization between devices")
                        multiline: true
                    }
                }
                
                Container {
                    visible: checkPermissions
                    preferredHeight: 20
                }
                Divider { visible: checkPermissions }
                
                Label {
                    visible: checkPermissions
                    text: qsTr("Google Drive")
                }
                
                Container {
                    visible: checkPermissions
                    preferredHeight: 20
                }
                
                Label {
                    visible: checkPermissions
                    text: qsTr("Full access. This enable you to see all your files, open them within Hg10, and share them with your friends.")
                    multiline: true
                }
                Label {
                    visible: checkPermissions
                    text: qsTr("Restricted access. Hg10 can only list and access to the files it has created. ")
                    multiline: true
                }
                Label {
                    visible: checkPermissions
                    text: qsTr("Deny access. Hg10 cannot access to Google Drive. ")
                    multiline: true
                }
                
                Container {
                    preferredHeight: 20
                    visible: checkPermissions
                }
                DropDown {
                    id: driveAccess
                    title: qsTr("Google drive")
                    visible: checkPermissions
                    
                    options: [
                        Option {
                            text: qsTr("Full access")
                            value: 1
                            selected: true
                        },
                        Option {
                            text: qsTr("Restricted access")
                            value: 2
                        },
                        Option {
                            text: qsTr("Deny access")
                            value: 3
                        }
                    ]
                }
                
                Divider { visible: checkPermissions }
                
                Button {
                    visible: checkPermissions
                    text: qsTr("Request permissions")
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        googleConnect.gmailAccess = gmailAccess.checked;
                        googleConnect.driveAccess = driveAccess.selectedValue;
                        checkPermissions = false;
                        googleConnect.logInRequest();
                    }
                }
                
                WebView {
                    visible: !checkPermissions
                    id: googleConnectView
                    
                    settings.textAutosizingEnabled: false
                    settings.zoomToFitEnabled: false
                
                }
            }
            
        }
        
        attachedObjects: [
            GoogleConnectController {
                id: googleConnect
                
                onCloseConnect: {
                    connectionMethod.visible = false;
                    submitButton.visible = false;
                    navSettings.pop();
                    connectingActivity.start();
                }
                
                onContactInfoObtained: {
                    if(!(typeof loginController === 'undefined')) {
                        console.log("Login request -- oath2")
                        loginController.oauth2Login();
                    }

                }
            }
        ]
        
        onCreationCompleted: {
            checkPermissions = true;
            googleConnect.setWebView(googleConnectView);
            
        }
    }
}
