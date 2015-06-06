import bb.cascades 1.3

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
                        color: Color.Black
                        base: SystemDefaults.TextStyles.PrimaryText
                    }
                }
                
                TextField {
                    id: host
                    hintText: qsTr("Host (auto)")
                    textStyle {
                        color: Color.Black
                    }
                    backgroundVisible: false
                }
                
                TextField {
                    id: domain
                    hintText: qsTr("Domain (auto)")
                    textStyle {
                        color: Color.Black
                    }
                    backgroundVisible: false
                }
                
                TextField {
                    id: port
                    hintText: qsTr("Port (auto)")
                    inputMode: TextFieldInputMode.Pin
                    textStyle {
                        color: Color.Black
                    }
                    backgroundVisible: false
                }
                
                TextField {
                    id: login
                    hintText: qsTr("Login")
                    textStyle {
                        color: Color.Black
                    }
                    backgroundVisible: false
                }
                
                TextField {
                    id: password
                    hintText: qsTr("Password")
                    inputMode: TextFieldInputMode.Password
                    backgroundVisible: false
                    textStyle {
                        color: Color.Black
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
                
                Container {
                    preferredHeight: ui.du(1)
                }
                
                CheckBox {
                    margin.leftOffset: ui.du(1)
                    margin.rightOffset: ui.du(1)
                    id: proxy
                    text: qsTr("Internal Server (Workspace)")
                    
                }
                
                Container {
                    preferredHeight: ui.du(1)
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
                                loginController.advancedLogin(host.text, domain.text, port.text, login.text,password.text, encryption.selectedValue, proxy.checked);
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
