import bb.cascades 1.2
import Network.LoginController 1.0

Page {
    signal done ()
        
    Container {
        id: rootContainer
        background: back.imagePaint
        layout: DockLayout {
        }
        
        ActivityIndicator {
            id: connectingActivity
            preferredHeight: 80
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Fill
            accessibility.name: "connectingActivity"
        }
        
        Container {
            verticalAlignment: VerticalAlignment.Center    
            
            Label {
                id: label
                text: "Hg10"
                textStyle {
                    color: Color.White
                    base: SystemDefaults.TextStyles.PrimaryText
                }
            }

            TextField {
                id: login
                hintText: qsTr("Login")
                textStyle {
                    color: Color.White
                }
                backgroundVisible: false
            }
            
            TextField {
                id: password
                hintText: qsTr("Password")
                inputMode: TextFieldInputMode.Password
                backgroundVisible: false
                textStyle {
                    color: Color.White
                }
                
            }
            
            Button {
                text: qsTr("Submit")
                horizontalAlignment: HorizontalAlignment.Center
                onClicked: {
                    connectingActivity.start();
                    
                    if(login.text != "")
                        loginController.login(login.text,password.text);
                }
            }
        }
        
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill
                
                
                imageSource: "asset:///images/wallpaper/wallpaper.jpg"
            }
        ]
    }
    
    attachedObjects: [
        LoginController {
            id: loginController
            
            onComplete: {
                connectingActivity.stop();
                done();
            }
            
            onConnectError: {
                connectingActivity.stop();
            }
        }
    ]
}
