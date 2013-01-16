import bb.cascades 1.0

Page {
    property string mac: "AABBCCDDEEFF" // dummy value that gets replaced
    property int macTab: 0              // dummy value that gets replaced
    Container {
        background: Color.DarkGray//Color.create("#ff909090")
        layout: StackLayout {}
        
	    /////
	    /////
	    /////
	    /////
	    /////
	    /////
	    Label {
            horizontalAlignment: HorizontalAlignment.Center
            text: "First 6 digits of MAC"
        }
	    Container {
	        
	        layout: StackLayout {
		        orientation: LayoutOrientation.LeftToRight
		    }
		    
		    horizontalAlignment: HorizontalAlignment.Center
            Container {
                layout: StackLayout {
    		        orientation: LayoutOrientation.TopToBottom
    		    }
	            Button {
	                text: "+"
	                onClicked: {mainPane.macIncr(macTab,12)}
	                maxWidth: 8
	            }
	
	            Label {
	                horizontalAlignment: HorizontalAlignment.Center
	                text: mac.charAt(12-12)
	            }
	            Button {
	                text: "-"
	                onClicked: {mainPane.macDecr(macTab,12)}
	                maxWidth: 8
	            }
	        }
		    Container {
	            Button {
	                text: "+"
	                onClicked: {mainPane.macIncr(macTab,11)}
	                maxWidth: 8
	            }
	
	            Label {
	                text: mac.charAt(12-11)
	                horizontalAlignment: HorizontalAlignment.Center
	            }
	            Button {
	                text: "-"
	                onClicked: {mainPane.macDecr(macTab,11)}
	                maxWidth: 8
	            }
	        }
            Container {
                Button {
                    text: "+"
                    onClicked: {mainPane.macIncr(macTab,10)}
                    maxWidth: 8
                }
                
                Label {
                    text: mac.charAt(12-10)
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Button {
                    text: "-"
                    onClicked: {mainPane.macDecr(macTab,10)}
                    maxWidth: 8
                }
            }
            Container {
                Button {
                    text: "+"
                    onClicked: {mainPane.macIncr(macTab,9)}
                    maxWidth: 8
                }
                
                Label {
                    text: mac.charAt(12-9)
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Button {
                    text: "-"
                    onClicked: {mainPane.macDecr(macTab,9)}
                    maxWidth: 8
                }
            }
            Container {
                Button {
                    text: "+"
                    onClicked: {mainPane.macIncr(macTab,8)}
                    maxWidth: 8
                }
                
                Label {
                    text: mac.charAt(12-8)
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Button {
                    text: "-"
                    onClicked: {mainPane.macDecr(macTab,8)}
                    maxWidth: 8
                }
            }
            Container {
                Button {
                    text: "+"
                    onClicked: {mainPane.macIncr(macTab,7)}
                    maxWidth: 8
                }
                
                Label {
                    text: mac.charAt(12-7)
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Button {
                    text: "-"
                    onClicked: {mainPane.macDecr(macTab,7)}
                    maxWidth: 8
                }
            }
	        
         }   
         
         
         
         Label {
             horizontalAlignment: HorizontalAlignment.Center
             text: "Last 6 digits of MAC"
         }
         Container {
             
             layout: StackLayout {
                 orientation: LayoutOrientation.LeftToRight
             }
             horizontalAlignment: HorizontalAlignment.Center
             Container {
                 layout: StackLayout {
                     orientation: LayoutOrientation.TopToBottom
                 }
                 Button {
                     text: "+"
                     onClicked: {mainPane.macIncr(macTab,6)}
                     maxWidth: 8
                 }
                 
                 Label {
                     horizontalAlignment: HorizontalAlignment.Center
                     text: mac.charAt(12-6)
                 }
                 Button {
                     text: "-"
                     onClicked: {mainPane.macDecr(macTab,6)}
                     maxWidth: 8
                 }
             }
             Container {
                 Button {
                     text: "+"
                     onClicked: {mainPane.macIncr(macTab,5)}
                     maxWidth: 8
                 }
                 
                 Label {
                     text: mac.charAt(12-5)
                     horizontalAlignment: HorizontalAlignment.Center
                 }
                 Button {
                     text: "-"
                     onClicked: {mainPane.macDecr(macTab,5)}
                     maxWidth: 8
                 }
             }
             Container {
                 Button {
                     text: "+"
                     onClicked: {mainPane.macIncr(macTab,4)}
                     maxWidth: 8
                 }
                 
                 Label {
                     text: mac.charAt(12-4)
                     horizontalAlignment: HorizontalAlignment.Center
                 }
                 Button {
                     text: "-"
                     onClicked: {mainPane.macDecr(macTab,4)}
                     maxWidth: 8
                 }
             }
             Container {
                 Button {
                     text: "+"
                     onClicked: {mainPane.macIncr(macTab,3)}
                     maxWidth: 8
                 }
                 
                 Label {
                     text: mac.charAt(12-3)
                     horizontalAlignment: HorizontalAlignment.Center
                 }
                 Button {
                     text: "-"
                     onClicked: {mainPane.macDecr(macTab,3)}
                     maxWidth: 8
                 }
             }
             Container {
                 Button {
                     text: "+"
                     onClicked: {mainPane.macIncr(macTab,2)}
                     maxWidth: 8
                 }
                 
                 Label {
                     text: mac.charAt(12-2)
                     horizontalAlignment: HorizontalAlignment.Center
                 }
                 Button {
                     text: "-"
                     onClicked: {mainPane.macDecr(macTab,2)}
                     maxWidth: 8
                 }
             }
             Container {
                 Button {
                     text: "+"
                     onClicked: {mainPane.macIncr(macTab,1)}
                     maxWidth: 8
                 }
                 
                 Label {
                     text: mac.charAt(12-1)
                     horizontalAlignment: HorizontalAlignment.Center
                 }
                 Button {
                     text: "-"
                     onClicked: {mainPane.macDecr(macTab,1)}
                     maxWidth: 8
                 }
             }
         
         }
         /////
         /////
         /////
         /////
         /////
         /////
         Button {
             id: button1
             topMargin: 85
             text: "Wake up!";
             horizontalAlignment: HorizontalAlignment.Center
             layoutProperties: StackLayoutProperties {}
             onClicked: {
                 mainPane.sendMacToCPP(macTab);
             }
         }
         /*
         Button {
	        id: button2
	        text: "XBMC remote"
	        horizontalAlignment: HorizontalAlignment.Center
	        layoutProperties: StackLayoutProperties {}
	        //onClicked: {
	        //    mainPane.pageStack.push(Qt.resolvedUrl("XbmcUI.qml"));
	        //}
	    }
	    */
		    
	}
}
