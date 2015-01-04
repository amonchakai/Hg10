import bb.cascades 1.2
import Network.LoginController 1.0
import Lib.QTimer 1.0

NavigationPane {
    id: navSettings
    property variant googlePage
    property variant facebookPage
        
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
                id: advancedSettingsButton
                defaultImageSource: "asset:///images/icon_settings.png"
                verticalAlignment: VerticalAlignment.Top
                horizontalAlignment: HorizontalAlignment.Right
                onClicked: {
                    advancedConnectionSettings.open();
                }
            }
            
            ImageButton {
                id: closeButton
                visible: false
                defaultImageSource: "asset:///images/Tutorial/closeIcon.png"
                verticalAlignment: VerticalAlignment.Top
                horizontalAlignment: HorizontalAlignment.Right
                onClicked: {
                    welcome.close();
                }
            }
            
            Container {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Fill    
                
                Label {
                    id: label
                    text: "Hg10"
                    textStyle {
                        color: Color.Black
                        base: SystemDefaults.TextStyles.PrimaryText
                    }
                }
                
                DropDown {
                    id: connectionMethod
                    visible: false
                    options: [
                        Option {
                            text: qsTr("Google (Secure)")
                            value: 0
                        },/*
                        Option {
                            text: qsTr("Facebook (Secure)")
                            value: 2
                        },*/
                        Option {
                            text: qsTr("Other")
                            value: 1
                        }
                    ]
                    title: qsTr("Connection method")
                    selectedIndex: 0
                    
                }
    
                
                Container {
                    preferredHeight: 60
                }
                
                Button {
                    id: submitButton
                    text: qsTr("Connect")
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked: {
                        if(connectionMethod.selectedValue == 0) {
                            if(!googlePage)
                                googlePage = googleConnect.createObject();
                            
                            navSettings.push(googlePage);
                        }
                        
                        if(connectionMethod.selectedValue == 2) { 
                            if(!facebookPage)
                                facebookPage = facebookConnect.createObject();
                            navSettings.push(facebookPage);
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
                    repeatPattern: RepeatPattern.XY
                    
                    
                    imageSource: "asset:///images/wallpaper/wallpaper.jpg"
                }
            ]
        }
        
        attachedObjects: [
            LoginController {
                id: loginController
                
                onComplete: {
                    connectingActivity.stop();
                    manual.open();
                }
                
                onConnectError: {
                    wasAnError = true;
                    connectingActivity.stop();
                    
                }
            },
            Sheet {
                id: manual
                Manual {
                    onDone: {
                        manual.close();
                        welcome.close();
                    }
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
            },
            
            
            ComponentDefinition {
                id: facebookConnect
                source: "FacebookConnect.qml"
            }
            
            
            
            
        ]
    }
    
    onPopTransitionEnded: {
        
    }
}
