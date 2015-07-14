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
                done();
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
                
                ScrollView {
                    
                    scrollViewProperties.scrollMode: ScrollMode.Horizontal
                    id: scrollView
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        
                        ImageView {
                            imageSource: "asset:///images/Tutorial/splash_720x720.jpg"
                            preferredHeight: widthFrame
                            preferredWidth: widthFrame
                        }
                        
                        Container {
                            preferredHeight: widthFrame
                            preferredWidth: widthFrame
                            
                            layout: DockLayout {
                            
                            }
                            
                            Label {
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Center
                                text: qsTr("Welcome to Hg10!")
                            }
                        }
                        
                        Container {
                            preferredHeight: widthFrame
                            preferredWidth: widthFrame
                            
                            layout: DockLayout {
                            
                            }
                            
                            Button {
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Center
                                text: qsTr("Done!")
                                onClicked: {
                                    done();
                                }
                            }
                        
                        }
                    
                    }
                    
                    onViewableAreaChanged: {
                        scrollView.scrollToPoint(Math.floor((viewableArea.x+widthFrame/2)/widthFrame)*widthFrame, 0)
                        index = Math.floor((viewableArea.x+widthFrame/2)/widthFrame);
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
                }
            }
        ]   
    }
}
