import bb.cascades 1.3

Page {
    signal done ()
    property int nbDots
    property int index
    property int widthFrame
    property int heightFrame
    
    Container {
        layout: DockLayout {
        }
        id: overallContainer
        
        Container {
            verticalAlignment: VerticalAlignment.Center
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                ScrollView {
                    
                    scrollViewProperties.scrollMode: ScrollMode.Horizontal
                    id: scrollView
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        
                        preferredHeight: heightFrame
                        
                        
                        ImageView {
                            imageSource: "asset:///images/Tutorial/splash_720x720.jpg"
                            preferredHeight: widthFrame
                            preferredWidth: widthFrame
                            verticalAlignment: VerticalAlignment.Center
                        }
                        
                        Container {
                            preferredHeight: heightFrame
                            preferredWidth: widthFrame
                            
                            layout: DockLayout { }
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Center
                            
                            Container {
                                layout: DockLayout { }
                                id: page2
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Center
                                
                                preferredHeight: ui.du(20)
                                preferredWidth: widthFrame
                                background: Color.create("#0092cc")
                                
                                Label {
                                    horizontalAlignment: HorizontalAlignment.Center
                                    verticalAlignment: VerticalAlignment.Center
                                    text: qsTr("Welcome to Hg10!")
                                    textStyle.fontSize: FontSize.Large
                                    textStyle.color: Color.White
                                }
                            }
                            
                            Label {
                                id: page2Content
                                text: qsTr(" You are now connected to the messaging service. \n\n Any issues can be reported in the bug report center. Please check existing issues before posting a new one.\n\n Encryption can be enabled from the settings in the application menu. "
                                + "\n\n I hope you will enjoy Hg10!")
                                textStyle.textAlign: TextAlign.Justify
                                verticalAlignment: VerticalAlignment.Top
                                visible: false
                                multiline: true
                            }
                        
                        }
                        
                        Container {
                            preferredHeight: heightFrame
                            preferredWidth: widthFrame
                            
                            layout: DockLayout {
                            
                            }
                            verticalAlignment: VerticalAlignment.Center
                            
                            Button {
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Center
                                text: qsTr("Start!")
                                onClicked: {
                                    done();
                                }
                            }
                        
                        }
                    
                    }
                    
                    
                    onViewableAreaChanged: {
                        scrollView.scrollToPoint(Math.floor((viewableArea.x+widthFrame/2)/widthFrame)*widthFrame, 0)
                        index = Math.floor((viewableArea.x+widthFrame/2)/widthFrame);
                        
                        if(index == 1) {
                            page2.verticalAlignment = VerticalAlignment.Top;
                            page2Content.visible = true;
                            page2Content.verticalAlignment = VerticalAlignment.Center;
                            
                            overallContainer.background = Color.White;
                        }
                        
                        
                    
                    }
                
                }
            }
            
            
            Label {
                id: caption
                horizontalAlignment: HorizontalAlignment.Center
                text: (index+1).toString() + "/" + nbDots.toString()
                textStyle {
                    color: Color.White
                }
            
            }
        }
        
        onCreationCompleted: {
            index = 0;
            nbDots = 3;
        }
        
        
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill

                imageSource: "asset:///images/wallpaper/wallpaper.jpg"
            },
            LayoutUpdateHandler {
                id: layoutUpdate
                onLayoutFrameChanged: {
                    widthFrame = layoutFrame.width;
                    heightFrame = layoutFrame.height
                }
            }
        ]   
        
        function goNext() {
            if(index == 1) {
                overallContainer.background = back.imagePaint
            } 
            
            scrollView.scrollToPoint((index+1)*widthFrame, 0)
        }
    }
    
    actions: [
        ActionItem {
            title: qsTr("Next")
            imageSource: "asset:///images/Tutorial/icon_play.png"
            ActionBar.placement: ActionBarPlacement.Signature
            
            onTriggered: {
                overallContainer.goNext();
            }
        }
    ]
}
