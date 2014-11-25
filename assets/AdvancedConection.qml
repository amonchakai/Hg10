import bb.cascades 1.2

Page {
    signal done();
    
    
    
    Container {
        background: back.imagePaint
            
        layout: DockLayout { }
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        
        ImageButton {
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Right
            defaultImageSource: "asset:///images/Tutorial/closeIcon.png"
            preferredHeight: 100
            preferredWidth: 100
            
            onClicked: {
                advancedConnectionSettings.close();
            }
        }
            
        ScrollView {
            verticalAlignment: VerticalAlignment.Center
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
                    id: host
                    hintText: qsTr("Host (auto)")
                    textStyle {
                        color: Color.White
                    }
                    backgroundVisible: false
                }
                
                TextField {
                    id: domain
                    hintText: qsTr("Domain (auto)")
                    textStyle {
                        color: Color.White
                    }
                    backgroundVisible: false
                }
                
                TextField {
                    id: port
                    hintText: qsTr("Port (auto)")
                    inputMode: TextFieldInputMode.Pin
                    textStyle {
                        color: Color.White
                    }
                    backgroundVisible: false
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
                
                DropDown {
                    id: encryption
                    options: [
                        Option {
                            text: "TLSEnabled"
                            value: 0
                        },
                        Option {
                            text: "TLSDisabled"
                            value: 1
                        },
                        Option {
                            text: "TLSRequired"
                            value: 2
                        }
                    ]
                    selectedIndex: 0
                    title: qsTr("Encryption")
                    horizontalAlignment: HorizontalAlignment.Center
                }
                
                Button {
                    text: qsTr("Submit")
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked: {
                        
                        if(host.text == "" && domain.text == "") {
                            loginController.login(login.text,password.text);
                            connectingActivity.start();
                            timer.start();
                        } else {
                            if(login.text != "") {
                                loginController.advancedLogin(host.text, domain.text, port.text, login.text,password.text, encryption.selectedValue);
                                connectingActivity.start();
                            }
                        }
                        advancedConnectionSettings.close();
                        
                    }
                }
                
            }
        }
    }
}
