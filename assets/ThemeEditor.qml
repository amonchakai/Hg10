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
                navSettings.pop();
            }
        }
    }
    
    ScrollView {        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            WebView {
                id: preview
                preferredHeight: 250
            }
            
            Divider { }
            
            Button {
                text: qsTr("Preview")
                horizontalAlignment: HorizontalAlignment.Fill
                onClicked: {
                    themeEditor.updateView();
                }
            }
            
            Divider { }
                        
            TextArea {
                id: editor
                
            }
            
        }
    }
    
    
    onCreationCompleted: {
        themeEditor.setWebView(preview);
        themeEditor.setEditor(editor);
        themeEditor.updateView();
    }
    
    attachedObjects: [
        ThemeEditorController {
            id: themeEditor
        }
    ]

}
