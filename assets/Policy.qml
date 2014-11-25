import bb.cascades 1.2

Page {
    signal done()
    titleBar: TitleBar {
        title: qsTr("Policy")
        dismissAction: [
            ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    done();
                }
            }
        ]
    }
    ScrollView {   
        WebView {
            url: "http://amonchakai.github.io/Hg10/"
            
            onNavigationRequested: {
                if(request.navigationType != WebNavigationType.Other) {
                    request.action = WebNavigationRequestAction.Ignore;
                    linkInvocation.query.uri = request.url;
                    
                    
                }
            }
        }
    }
    
    attachedObjects: [
        Invocation {
            id: linkInvocation
            
            query.invokeTargetId: "sys.browser";
            query.invokeActionId: "bb.action.OPEN"
            
            
            query {
                onUriChanged: {
                    linkInvocation.query.updateQuery();
                }
            }
            
            onArmed: {
                
                trigger("bb.action.OPEN");
            }
        }
    ]
}
