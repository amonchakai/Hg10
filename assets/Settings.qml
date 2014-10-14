import bb.cascades 1.2
import Network.LoginController 1.0

NavigationPane {
    id: nav
    property variant tpage
    property variant colorPage
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
                    nav.done();
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Save")
                onTriggered: {
                    //appSettings.saveSettings(); 
                    nav.done();
                }
            }
        }
        
        ScrollView {
            id: settingPage
            property string userName;
            
    	    Container {
    	        layout: StackLayout {}
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
                    background: headerContainer.themeStyleToHeaderColor(Application.themeSupport.theme.colorTheme.style)
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    preferredHeight: 40
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Label {
                        text: qsTr("Login settings")
                        textStyle.fontSize: FontSize.XSmall
                    }
                } 
                Container {
                    background: Color.create("#0089bf") 
                    minHeight: 4
                    maxHeight: 4
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                
    	        Label {
    	            id: userLabel
                    text: qsTr("User: ") + settingPage.userName
                }
    
    	        
    	        // Commit button
    	        Button {
    	            id: loginButton
    	            text: qsTr("Connect")
    	            horizontalAlignment: HorizontalAlignment.Fill
    	            onClicked: {
                        if(!tpage)
                        	tpage = loginPage.createObject();
                        nav.push(tpage);
    	            }
    	            visible: !loginController.isLogged()
    	        }
    	        
    	        
    	        Button {
    	            id: logOutButton
    	            text: qsTr("log out");
    	            horizontalAlignment: HorizontalAlignment.Fill
    	            onClicked: {
    	                loginController.logOut();
    	                loginButton.setVisible(true);
    	                logOutButton.setVisible(false);
                        userLabel.setText(qsTr("User: "));
    	            }
    	            visible: loginController.isLogged()
    	        }
    	        
    	        
                Button {
                    id: clearHistory
                    text: qsTr("Clear history");
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        loginController.deleteHistory();
                    }
                }
    	    }
    	    
    	    
    	    attachedObjects: [
                LoginController {
                    id: loginController
                },
                ComponentDefinition {
                    id: loginPage
                    source: "LoginForm.qml"
                
                }
            ]
    	    
            onUserNameChanged: {
                console.debug("name changed");
                userLabel.setText(qsTr("User: ") + userName);
            }
    	}
    } 
	
	onCreationCompleted: {
        settingPage.userName =  loginController.savedlogin;
    }
	
	onPopTransitionEnded: {
	    
        userLabel.setText(qsTr("User: ") + loginController.savedlogin);
        loginButton.setVisible(!loginController.isLogged());
        logOutButton.setVisible(loginController.isLogged());
    }
	
}
