import bb.cascades 1.2
import Network.LoginController 1.0

Page {
    titleBar: TitleBar {
        id: pageTitleBar
        title: qsTr("Login")
        
        acceptAction: ActionItem {
            title: qsTr ("Log")
            
            onTriggered: {
                loginController.login(loginField.text,passwordField.text)
            }
        }
        
        dismissAction: ActionItem {
            title: qsTr ("Cancel")
            
            onTriggered: {
                nav.pop()
            }
        }
    }
    
    Container {
        
        // Login + Text field
        Label {
            text: qsTr("Login")
        }
        TextField {
            id: loginField
            text: loginController.savedlogin
            accessibility.name: "login"
        }   
        
        
        // Password + Text field
        Label {
            text: qsTr("Password")
        }
        TextField {
            id: passwordField
            inputMode: TextFieldInputMode.Password
            accessibility.name: "password"
        }
        
        attachedObjects: [
            LoginController {
                id: loginController
                
                onComplete: {
                    nav.pop()
                }
            }
        ]
    }
}
