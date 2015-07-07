import bb.cascades 1.3
import Network.LoginController 1.0
import conf.SettingsController 1.0
import com.netimage 1.0

NavigationPane {
    id: navSettings
    property int depth
    property variant tpage
    property variant googlePage
    property variant dropboxPage
    property variant logPage
    property variant wallpaperPage
    signal done ()
    
	Page {
        id: settingsPage
        
        
        titleBar: TitleBar {
            title: qsTr("Settings")
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    // Emit the custom signal here to indicate that this page needs to be closed
                    // The signal would be handled by the page which invoked it
                    settings.close();
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Save")
                onTriggered: {
                    settingsController.save(); 
                    settings.close();
                }
            }
        }
        
        ScrollView {
            id: settingPage
            property string userName;
            
    	    Container {
    	        layout: StackLayout {
    	            orientation: LayoutOrientation.TopToBottom
    	        }
                id: headerContainer
                horizontalAlignment: HorizontalAlignment.Fill
    	        
                function themeStyleToHeaderColor(style){
                    switch (style) {
                        case VisualStyle.Bright:
                            return Color.create(0.96,0.96,0.96);
                        case VisualStyle.Dark: 
                            return Color.create(0.15,0.15,0.15);
                        default :
                            return Color.create(0.96,0.96,0.96);    
                    }
                    return Color.create(0.96,0.96,0.96); 
                }
    	        
                // --------------------------------------------------------------------------
                // Login settings
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Profile")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                Container {
                    layout: DockLayout { }
                    background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#131313") : Color.create("#fafafaff")
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Left
                        
                        Container {
                            preferredWidth: ui.du(.1)
                        }
                        
                        
                        ImageView {
                            imageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/avatar_white.png" : "asset:///images/avatar_black.png"
                            preferredHeight: ui.du(3)
                            preferredWidth: ui.du(3)
                            verticalAlignment: VerticalAlignment.Center
                            scalingMethod: ScalingMethod.AspectFit
                        
                        }
                        
                        TextField {
                            enabled: false
                            id: userLabel
                            text: settingsController.userName
                            verticalAlignment: VerticalAlignment.Center
                        
                        }
                    
                    }
                    
                    
                    
                    // Commit button
                    Button {
                        text: qsTr("Connect")
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        preferredWidth: ui.du(30)
                        id: loginButton
                        onClicked: {
                            welcome.open();
                        }
                        visible: (settingsController.userName == "")
                    }
                    
                    
                    Button {
                        id: logOutButton
                        text: qsTr("log out");
                        preferredWidth: ui.du(30)
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        
                        onClicked: {
                            loginController.logOut();
                            loginButton.setVisible(true);
                            logOutButton.setVisible(false);
                            userLabel.setText(qsTr("User: "));
                        }
                        visible: (settingsController.userName != "")
                    }
                
                }
                
                // --------------------------------------------------------------------------
                // Theme setting
                

                Container {
                    preferredHeight: ui.du(4)
                }

                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Visual appearance")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                
                
                DropDown {
                    id: theme
                    title: qsTr("Visual Theme")
                    options: [
                        Option {
                            text: qsTr("Bright")
                            value: 1
                        },
                        Option {
                            text: qsTr("Dark")
                            value: 2
                        } 
                    ]
                    selectedIndex: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? 1 : 0
                    onSelectedOptionChanged: {
                        settingsController.theme = theme.selectedOption.value;
                    }
                
                } 
                
                DropDown {
                    id: fontSize
                    title: qsTr("Font size")
                    options: [
                        Option {
                            text: "25"
                            value: 25
                        },
                        Option {
                            text: "28"
                            value: 28
                        },
                        Option {
                            text: "31"
                            value: 31
                        },
                        Option {
                            text: "34"
                            value: 34
                        },
                        Option {
                            text: "37"
                            value: 37
                        },
                        Option {
                            text: "40"
                            value: 40
                        }
                    ]
                    selectedIndex: (settingsController.fontSize - 25) / 3
                    onSelectedOptionChanged: {
                        settingsController.fontSize = fontSize.selectedOption.value;
                    }
                    
                    
                }
                
                Button {
                    id: wallpaperButton
                    text: qsTr("Themes")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        if(!wallpaperPage)
                            wallpaperPage = wallpaperSettings.createObject();
                        navSettings.push(wallpaperPage);
                    }
                }
                
                DropDown {
                    id: convers
                    title: qsTr("Chat interface")
                    options: [
                        Option {
                            text: qsTr("Native")
                            value: 0
                        },
                        Option {
                            text: qsTr("Custom")
                            value: 1
                        } 
                    ]
                    selectedIndex: settingsController.conversTheme
                    onSelectedOptionChanged: {
                        settingsController.conversTheme = convers.selectedOption.value;
                    }
                
                } 
                
                Container {
                    preferredHeight: ui.du(4)
                }
                
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Encryption")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                Label {
                    multiline: true
                    textStyle.textAlign: TextAlign.Justify
                    textStyle.fontSize: FontSize.Small
                    margin.leftOffset: ui.du(1)
                    margin.rightOffset: ui.du(1)
                    text: qsTr("End-to-end encryption requires your contact to have a client which support encryption. Desktop clients include Adium, Pidgin, Kopete... ChatSecure on mobile. The official Google Hangouts client do NOT support it.")
                }
                
                Button {
                    text: qsTr("Setup encryption keys")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        settingsController.setupKeys();
                    }
                }
                
                Button {
                    text: qsTr("Show your fingerprint")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        settingsController.showKeys();
                    }
                }
                
                Container {
                    layout: DockLayout {
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Display warning about encryption")
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Left
                        margin.leftOffset: ui.du(1)
                    }
                    
                    ToggleButton {
                        id: otrWarning
                        checked: settingsController.otrWarning
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        
                        onCheckedChanged: {
                            settingsController.otrWarning = checked;
                        }
                        margin.rightOffset: ui.du(1);
                    }
                
                }
                
    	        
                Container {
                    preferredHeight: ui.du(5)
                }
                
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("File hosting")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                
    	        
    	        Container {
    	            layout: DockLayout {
    	                
    	            }
    	            horizontalAlignment: HorizontalAlignment.Fill
    	            
    	            Label {
                        text: qsTr("Use dropbox to host files")
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Left
                        margin.leftOffset: ui.du(1)
    	            }
    	            
    	            ToggleButton {
    	                id: useDropbox
                        checked: settingsController.useDropbox
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        
                        onCheckedChanged: {
                            settingsController.useDropbox = checked;
                        }
                        margin.rightOffset: ui.du(1);
    	            }
    	            
    	        }
    	                            	        
                Button {
                    id: dropboxConnectButton
                    text: qsTr("Log to dropbox")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        if(!dropboxPage)
                            dropboxPage = dropboxConnect.createObject();
                        navSettings.push(dropboxPage);
                    }
                    
                }
                
                Container {
                    preferredHeight: ui.du(4)
                }
                
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Cache management")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
    	        
                Button {
                    id: clearHistory
                    text: qsTr("Clear history");
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        loginController.deleteHistory();
                    }
                }
                
                Button {
                    id: clearAudio
                    text: qsTr("Clear audio messages");
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        loginController.deleteAudioMessages();
                    }
                }
                
                Button {
                    id: clearvCard
                    text: qsTr("Clear stored contact information")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        loginController.clearContactsData();
                    }
                }
                
                
                
                Container {
                    preferredHeight: ui.du(4)
                }
                
                Container {
                    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                    
                    Label {
                        text: qsTr("Logs")
                        textStyle.fontSize: FontSize.Large
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                
                }
                Divider { }
                
                
                Container {
                    layout: DockLayout { }
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Enable logs")
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        margin.leftOffset: ui.du(1)
                    }
                
                    ToggleButton {
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        id: enableLogs
                        
                        checked: settingsController.enableLogs
                        onCheckedChanged: {
                            settingsController.enableLogs = checked;
                        }
                        margin.rightOffset: ui.du(1)
                    }    
                    
                }
                
                Button {
                    id: log
                    text: qsTr("Application Logs")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        if(!logPage)
                            logPage = applicationLog.createObject();
                        navSettings.push(logPage);
                    }
                    
                }
                
                Divider { }
                
                Button {
                    id: removeHub
                    visible: false
                    text: qsTr("Remove Hub")
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        settingsController.requestHubRemoval();
                    }
                }
                
    	    }
    	    
    	    
    	    attachedObjects: [
                LoginController {
                    id: loginController
                },
                SettingsController {
                    id: settingsController
                },
                ComponentDefinition {
                    id: googleConnect
                    source: "GoogleConnect.qml"
                },
                ComponentDefinition {
                    id: dropboxConnect
                    source: "DropboxConnect.qml"
                },
                ComponentDefinition {
                    id: applicationLog
                    source: "ApplicationLog.qml"
                },
                ComponentDefinition {
                    id: wallpaperSettings
                    source: "WallpaperSettings.qml"
                }
            ]
    	    
            onUserNameChanged: {
                console.debug("name changed");
                userLabel.setText(qsTr("User: ") + settingsController.userName);
            }
    	}
    } 
	
	onCreationCompleted: {
        settingPage.userName =  settingsController.userName;
        depth = 0;
    }
	
	onPopTransitionEnded: {
	    --depth;
        userLabel.setText(qsTr("User: ") + settingsController.userName);
        loginButton.setVisible(!loginController.isLogged());
        logOutButton.setVisible(loginController.isLogged());
        
        if(depth == 1) {
            wallpaperPage.destroy();
            wallpaperPage = null;
        }
    }
	
	onPushTransitionEnded: {
	    ++depth;
	}
	
}
