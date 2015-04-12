/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.3


NavigationPane {
    id: navBug
    signal done
    
    Page {
        id: bugReport
        property variant bpage
        
        Container {
            
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            
            Container {
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                horizontalAlignment: HorizontalAlignment.Fill            
                background: Color.create("#e66700")
                
                layout: DockLayout { }
                
                ImageButton {
                    id: closeButton
                    defaultImageSource: "asset:///images/Tutorial/closeIcon.png"
                    verticalAlignment: VerticalAlignment.Top
                    horizontalAlignment: HorizontalAlignment.Right
                    onClicked: {
                        done();
                    }
                    preferredHeight: ui.du(10)
                    preferredWidth: ui.du(10)
                }
                
                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    
                    Label {
                        text: qsTr("Hg10")
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle.fontSize: FontSize.XXLarge
                        textStyle.color: Color.White
                    }
                    
                    Label {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: qsTr("Bug report center")
                        textStyle.color: Color.White
                    }
                }
            }
            
            
            Container {
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 3
                }
                horizontalAlignment: HorizontalAlignment.Fill
                
                layout: GridLayout {
                    columnCount: 2
                }
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    ImageButton {
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/BugReport_white_large.png" : "asset:///images/BugReport.png"
                        horizontalAlignment: HorizontalAlignment.Center
                        
                        onClicked: {
                            if(!bugReport.bpage) {
                                bugReport.bpage = listIssues.createObject();
                            }
                            bugReport.bpage.typeIssue = 1;
                            bugReport.bpage.catImg = Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/BugReport_white_large.png" : "asset:///images/BugReport.png";
                            bugReport.bpage.category = qsTr("Bug");
                            navBug.push(bugReport.bpage);
                        }
                    }
                    Label {
                        text: qsTr("Bug")
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    ImageButton {
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/EnhancementRequest_white.png" : "asset:///images/EnhancementRequest.png" 
                        horizontalAlignment: HorizontalAlignment.Center
                        
                        onClicked: {
                            if(!bugReport.bpage) {
                                bugReport.bpage = listIssues.createObject();
                            }
                            bugReport.bpage.typeIssue = 2;
                            bugReport.bpage.catImg = Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/EnhancementRequest_white.png" : "asset:///images/EnhancementRequest.png" 
                            bugReport.bpage.category = qsTr("Enhancement");
                            navBug.push(bugReport.bpage);
                        }
                    
                    }
                    Label {
                        text: qsTr("Enhancement")
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
                
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    ImageButton {
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/HelpRequest_white.png" : "asset:///images/HelpRequest.png" 
                        horizontalAlignment: HorizontalAlignment.Center
                        
                        onClicked: {
                            if(!bugReport.bpage) {
                                bugReport.bpage = listIssues.createObject();
                            }
                            bugReport.bpage.typeIssue = 3;
                            bugReport.bpage.catImg = Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/HelpRequest_white.png" : "asset:///images/HelpRequest.png" ;
                            bugReport.bpage.category = qsTr("Need Help");
                            navBug.push(bugReport.bpage);
                        }
                    }
                    Label {
                        text: qsTr("Need Help")
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
                
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    ImageButton {
                        defaultImageSource: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/BlablaRequest_white.png" : "asset:///images/BlablaRequest.png"
                        horizontalAlignment: HorizontalAlignment.Center
                        
                        onClicked: {
                            if(!bugReport.bpage) {
                                bugReport.bpage = listIssues.createObject();         
                            }
                            bugReport.bpage.typeIssue = 4;
                            bugReport.bpage.catImg = Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? "asset:///images/BlablaRequest_white.png" : "asset:///images/BlablaRequest.png";
                            bugReport.bpage.category = qsTr("Comments");
                            navBug.push(bugReport.bpage);
                        
                        }
                    
                    }
                    Label {
                        text: qsTr("Questions")
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
            
            }
        
        }
        
        attachedObjects: [
            ComponentDefinition {
                id: listIssues
                source: "ListIssues.qml"   
            }
        ]
    }
}
