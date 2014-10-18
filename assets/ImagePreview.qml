import bb.cascades 1.2
import com.netimage 1.0

Page {
    property string imageList;
    
    Container {
        background: Color.Black
        layout: DockLayout {
        }
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        ActivityIndicator {
            id: activity
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: 80
        }
        
        ScrollView {
            id: scrollView
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            scrollViewProperties.scrollMode: ScrollMode.Both
           scrollViewProperties.pinchToZoomEnabled: true
        
	        ImageView {
	            id: imagePreview
	            horizontalAlignment: HorizontalAlignment.Center
	            verticalAlignment: VerticalAlignment.Center
	            scalingMethod: ScalingMethod.AspectFit
	            touchBehaviors: TouchBehavior {
	                TouchReaction {
	                    
	                }
	            }
	            property double initialScale: 1.0
	            property double scaleFactor: 0.8
	            
	            image: tracker.image
	            
	            attachedObjects: [
	                NetImageTracker {
	                    id: tracker
	                    loadingImage: ""
	                    
	                    onCompleted: {
                         	activity.stop();
                        }
	                } 
	            ]
	            
	        }
	    }
    }
    
    onImageListChanged: {
        activity.start();
        tracker.source = imageList;
    }
}
