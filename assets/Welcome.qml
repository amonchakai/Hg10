import bb.cascades 1.2
import Network.LoginController 1.0
import Lib.QTimer 1.0

NavigationPane {
    id: navSettings
    property variant googlePage
        
    Page {
        id: welcomePage
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
                defaultImageSource: "asset:///images/icon_settings_black.png"
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
                    
                
                Container {
                    preferredHeight: 60
                }
                
                Button {
                    id: submitButton
                    text: qsTr("Connect")
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked: {
                         if(!googlePage)
                             googlePage = googleConnect.createObject();
                         
                         navSettings.push(googlePage);                                                
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
                    welcomePage.wasAnError = true;
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
                    if(!welcomePage.wasAnError)
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
