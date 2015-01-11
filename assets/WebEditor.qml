import bb.cascades 1.2

Page {
    property string link
    Container {
        layout: DockLayout { }
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        
        ScrollView {
            
            WebView {
                 preferredHeight: 200000
                 verticalAlignment: VerticalAlignment.Fill
                 horizontalAlignment: HorizontalAlignment.Fill
                 settings.textAutosizingEnabled: false
                 settings.zoomToFitEnabled: false
                 id: webview
                 
             }
        }         
    }
        
    onLinkChanged: {
        webview.url = link;
    }
}
