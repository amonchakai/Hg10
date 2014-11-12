import bb.cascades 1.2

Page {
    signal done ()
    property int nbDots
    property int index
    property int widthFrame
    
    Container {
        background: back.imagePaint
        layout: DockLayout {
        }
        
        ImageButton {
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Right
            defaultImageSource: "asset:///images/Tutorial/closeIcon.png"
            preferredHeight: 100
            preferredWidth: 100
            
            onClicked: {
                welcome.close();
                manual.close();
            }
        }
        
        Container {
            verticalAlignment: VerticalAlignment.Center
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }

            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Container {
                    id: offset
                    preferredWidth: (widthFrame-720)/2
                }
                
                ScrollView {
                    
                    scrollViewProperties.scrollMode: ScrollMode.Horizontal
                    id: scrollView
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        
                        ImageView {
                            imageSource: "asset:///images/Tutorial/splash_720x720.jpg"
                            preferredHeight: 720
                            preferredWidth: 720
                        }
                        ImageView {
                            imageSource: "asset:///images/Tutorial/listContacts.jpg"
                            preferredHeight: 720
                            preferredWidth: 720
                        }
                        ImageView {
                            imageSource: "asset:///images/Tutorial/settings.jpg"
                            preferredHeight: 720
                            preferredWidth: 720
                        }
                        ImageView {
                            imageSource: "asset:///images/Tutorial/logIn.jpg"
                            preferredHeight: 720
                            preferredWidth: 720
                        }
                        ImageView {
                            imageSource: "asset:///images/Tutorial/accept.jpg"
                            preferredHeight: 720
                            preferredWidth: 720
                        }
                        ImageView {
                            imageSource: "asset:///images/Tutorial/chat.jpg"
                            preferredHeight: 720
                            preferredWidth: 720
                        }
                        ImageView {
                            imageSource: "asset:///images/Tutorial/listContactsFiltered.jpg"
                            preferredHeight: 720
                            preferredWidth: 720
                        }
                        Container {
                            preferredHeight: 720
                            preferredWidth: 720
                            
                            layout: DockLayout {
                                
                            }
                            
                            Button {
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Center
                                text: qsTr("Done!")
                                onClicked: {
                                    welcome.close();
                                    manual.close();
                                }
                            }
                            
                        }
                        
                        Container {
                            preferredWidth: (widthFrame-720)/2
                        }
                        
                    }
                    
                    onViewableAreaChanged: {
                        console.log(viewableArea.x)
                        scrollView.scrollToPoint(Math.floor((viewableArea.x+360)/720)*720, 0)
                        index = Math.floor((viewableArea.x+360)/720);
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
            nbDots = 8;
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
                }
            }
        ]   
    }
}
