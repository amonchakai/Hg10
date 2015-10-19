import bb.cascades 1.3

Page {
    titleBar: TitleBar {
        title: qsTr("Previous conversation")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                nav.pop();
            }
        }
    }
    
    property string id
    
    Container {
        layout: DockLayout {}
        id: wallpaperContainer
        
        Container {
            
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }        
            
            background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#000000") : Color.create("#c7dfe4")
            
            ActivityIndicator {
                id: linkStatusActivity
                preferredHeight: 50
                horizontalAlignment: HorizontalAlignment.Center
            }
            
            ScrollView {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                id: scrollView
                
                
                focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
                
                WebView {
                    id: messageView
                    
                    settings.background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#000000") : Color.create("#c7dfe4")
                    
                    
                    settings.textAutosizingEnabled: false
                    settings.zoomToFitEnabled: false
                    
                    html: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "<!DOCTYPE html><html><head><style>body { background-color: #000000; } </style></head><body></body></html>" : "" ;
                    
                    onLoadingChanged: {
                        if (loadRequest.status == WebLoadStatus.Succeeded) {
                            messageView.evaluateJavaScript("getPageHeight();");
                            txtField.requestFocus();
                        }
                    }
                    
                    onMessageReceived: {
                        var isScroll = RegExp("PAGE_HEIGHT:([0-9]+)")
                        var match = message.data.match(isScroll);
                        if(match)
                            scrollView.scrollToPoint(0, match[1], ScrollAnimation.None);
                        
                        console.log(message.data)
                    }
                    
                    onNavigationRequested: {
                        if(request.navigationType != WebNavigationType.Other) {
                            request.action = WebNavigationRequestAction.Ignore;
                            
                            console.log("on nav requested")
                            var urlImg = RegExp(".jpg");
                            var urlImgPng = RegExp(".png");
                            var urlImgGif = RegExp(".gif");
                            if(urlImg.test(request.url.toString()) || urlImgPng.test(request.url.toString()) || urlImgGif.test(request.url.toString()))
                                showPictureViewer(request.url);
                            else
                                conversationController.invokeBrowser(request.url.toString());
                        
                        } else { 
                            request.action = WebNavigationRequestAction.Accept;
                        }
                    }
                    
                }
            }
        }
    }
    
    onCreationCompleted: {
        historyBrowserController.setWebView(messageView);
    }
    
    onIdChanged: {
        historyBrowserController.loadThread(id);
    }
}
