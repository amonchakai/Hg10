import bb.cascades 1.3
import Network.ThemeEditorController 1.0

Page {
    property string userId
    
    titleBar: TitleBar {
        title: qsTr("Edit theme")
        dismissAction: ActionItem {
            title: qsTr("Close")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                navSettings.pop();
            }
        }
        
        acceptAction: ActionItem {
            title: qsTr("Save")
            onTriggered: {
                // Emit the custom signal here to indicate that this page needs to be closed
                // The signal would be handled by the page which invoked it
                themeEditor.backgroundColor = colorField.text;
                themeEditor.saveTheme();
                navSettings.pop();
            }
        }
    }
    
    ScrollView {        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            Container {
                id: backColor
                background: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.create("#000000") : Color.create("#c7dfe4")
                
                WebView {
                    id: preview
                    settings.textAutosizingEnabled: false
                    settings.zoomToFitEnabled: false
                    
                    preferredHeight: ui.du(40)
                    settings.background: Color.Transparent
                }
                
            }
            
            Divider { }
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Container {
                    preferredWidth: ui.du(1)
                }
                
                Label {
                    text: qsTr("Background color:")
                    verticalAlignment: VerticalAlignment.Center
                }
                
                TextField {
                    id: colorField
                    text: themeEditor.backgroundColor
                    verticalAlignment: VerticalAlignment.Center                    
                }
                
                Container {
                    preferredWidth: ui.du(1)
                }

                
            }
            
            Button {
                text: qsTr("Preview")
                horizontalAlignment: HorizontalAlignment.Fill
                onClicked: {
                    backColor.background = Color.create(colorField.text);
                    themeEditor.updateView();
                }
            }
            
            Divider { }
                        
            TextArea {
                id: editor
                
            }
            
        }
    }
    
    actions: [
        ActionItem {
            title: qsTr("Presets")
            imageSource: "asset:///images/icon_theme.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            
            onTriggered: {
                themeEditor.selectPreset();
            }
        }
    ]
    
    
    onCreationCompleted: {
        themeEditor.setWebView(preview);
        
    }
    
    onUserIdChanged: {
        themeEditor.userId = userId;
        themeEditor.setEditor(editor);
        themeEditor.updateView();
    }
    
    attachedObjects: [
        ThemeEditorController {
            id: themeEditor
            
            onColorSet: {
                colorField.text = value;
                backColor.background = Color.create(value);
            }
        }
    ]

}
