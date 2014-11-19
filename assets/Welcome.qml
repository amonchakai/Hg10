import bb.cascades 1.2
import Network.LoginController 1.0
import Lib.QTimer 1.0

NavigationPane {
    id: navSettings
    property variant googlePage
        
    Page {
        signal done ()
        
        property bool wasAnError
            
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
            
            ImageButton {
                defaultImageSource: "asset:///images/icon_settings.png"
                verticalAlignment: VerticalAlignment.Top
                horizontalAlignment: HorizontalAlignment.Right
                onClicked: {
                    advancedConnectionSettings.open();
                }
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
                
                DropDown {
                    id: connectionMethod
                    options: [
                        Option {
                            text: qsTr("Google (Secure)")
                            value: 0
                        },
                        Option {
                            text: qsTr("Other")
                            value: 1
                        }
                    ]
                    title: qsTr("Connection method")
                    
                    onSelectedOptionChanged: {
                        if(selectedOption.value == 1) {
                            login.visible = true;
                            password.visible = true;
                        } else {
                            login.visible = false;
                            password.visible = false;
                        }
                    }
                    selectedIndex: 0
                }
    
                TextField {
                    id: login
                    visible: false
                    hintText: qsTr("Login")
                    textStyle {
                        color: Color.White
                    }
                    backgroundVisible: false
                }
                
                TextField {
                    id: password
                    visible: false
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
                        if(connectionMethod.selectedValue == 1) {
                            if(login.text != "") {
                                loginController.login(login.text,password.text);
                                connectingActivity.start();
                                timer.start();
                                wasAnError = false;
                            }
                        } else {
                            if(!googlePage)
                                googlePage = googleConnect.createObject();
                            navSettings.push(googlePage);
                        }
                    }
                }
                
                Button {
                    id: stillWaitingButton
                    text: qsTr("Still waiting? :(")
                    visible: false
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked: {
                        furtherInformation.open();
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
                    if(connectionMethod.selectedOption.value == 1) {
                        connectingActivity.stop();
                        manual.open();
                    } else {
                        connectingActivity.stop();
                        welcome.close();
                    }
                    
                }
                
                onConnectError: {
                    wasAnError = true;
                    connectingActivity.stop();
                    
                }
            },
            Delegate {
                id: manualDelegate
                source: "Manual.qml"
                
            },
            Sheet {
                id: manual
                content: manualDelegate.object
                onOpenedChanged: {
                    if (opened)
                        manualDelegate.active = true;
                }
                
                onClosed: {
                    manualDelegate.active = false;
                }
            },
            QTimer {
                id: timer
                
                singleShot: true
                interval: 7000
                
                onTimeout: {
                    if(!wasAnError)
                        stillWaitingButton.visible = true;
                }
            },
            
            Delegate {
                id: furtherInformationDelegate
                source: "FurtherInformation.qml"
            },
            Sheet {
                id: furtherInformation
                content: furtherInformationDelegate.object
                onOpenedChanged: {
                    if (opened)
                        furtherInformationDelegate.active = true;
                }
                
                onClosed: {
                    furtherInformationDelegate.active = false;
                }
            },
            
            Delegate {
                id: advancedConnectionSettingsDelegate
                source: "AdvancedConection.qml"
            },
            Sheet {
                id: advancedConnectionSettings
                content: advancedConnectionSettingsDelegate.object
                onOpenedChanged: {
                    if (opened)
                        advancedConnectionSettingsDelegate.active = true;
                }
                
                onClosed: {
                    advancedConnectionSettingsDelegate.active = false;
                }
            },
            ComponentDefinition {
               id: googleConnect
               source: "GoogleConnect.qml"
            }
            
            
        ]
    }
    
    onPopTransitionEnded: {
        
    }
}
