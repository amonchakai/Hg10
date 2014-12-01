import bb.cascades 1.2
import Lib.ApplicationLogController 1.0

Page {
    ScrollView {
        WebView {
            id: appLogView
            
            settings.textAutosizingEnabled: false
            settings.zoomToFitEnabled: false
                    
        }        
    }
    
    actions: [
        DeleteActionItem {
            title: qsTr("Clear logs")
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                logger.clearLogs();
            }
        },
        ActionItem {
            title: qsTr("Refresh")
            imageSource: "asset:///images/icon_refresh.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                logger.showLogs();
            }
        }
    ]
    
    
    onCreationCompleted: {
        logger.setWebView(appLogView);
        logger.showLogs();
    }
    
    attachedObjects: [
        ApplicationLogController {
            id: logger
        }
    ]
}
