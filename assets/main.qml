// Tabbed Pane project template
import bb.cascades 1.0
import bb.system 1.0
import "wolmeup.js" as Wolmeupjs
import "storage.js" as Storage

TabbedPane {
    id: mainPane
    showTabsOnActionBar: true
    //
    // Signals and slots
    //
    
    signal macIncr(int macnum, int macdigit)
    signal macDecr(int macnum, int macdigit)
    signal updateMacs // update MACs
    signal sendMacToCPP(int macnum) // helper signal for sending MAC to broadCaster.cpp
    signal storeMACsToSQL // signal coming from main.cpp for settings in MainPage.qml
    signal setXBMCSettings // signal coming from main.cpp for settings in XbmcUI.qml

    property bool hasMACChanged: false;
    onMacDecr: {
        Wolmeupjs.macDecr(macnum, macdigit);
        mainPane.updateMacs();
        hasMACChanged = true;
    }
    onMacIncr: {
        Wolmeupjs.macIncr(macnum, macdigit);
        mainPane.updateMacs();
        hasMACChanged = true;
    }
    onUpdateMacs: { // get new MACs from JS
        tab1.mac = Wolmeupjs.macToStr(1);
        tab2.mac = Wolmeupjs.macToStr(2);
        //tab3.mac = Wolmeupjs.macToStr(3);
    }
    onSendMacToCPP: {
        BroadCaster.broadCastMAC(Wolmeupjs.macToStr(macnum));
    }
    onStoreMACsToSQL: {
        if(hasMACChanged){
            Storage.setSetting("mac1",tab1.mac);
            Storage.setSetting("mac2",tab2.mac);
            //Storage.setSetting("mac3",tab3.mac);
            hasMACChanged = false;
        }
    }
    onSetXBMCSettings: {
        xbmcUI.setXBMCSettings();
    }
    Tab {
        title: qsTr("MAC 1")
                MainPage {
            id: tab1
            //mac: Wolmeupjs.macToStr(1);
            macTab: 1;
            actions: [
                // define the actions for tab here
                ActionItem {
                    title: qsTr("About")
                    imageSource: "asset:///images/help-about.png"
                    attachedObjects: [
                        SystemDialog {
                            id: myMACdialog1
                            title: "Wake on LAN"
                            body: "Provide MAC address to wake up a computer in the same network. Two separate MAC addresses can be configured in the two tabs. The MAC address gets sent to the broadcast address port 9 (IPv4). The computer must have Wake-on-LAN enabled in BIOS.\n\nCode by Miika Ahdesmaki 2013. miika.ahdesmaki@ovi.com"
                            onFinished:{
                            } 
                        }
                    ]
                    onTriggered: {
                        // display help page
                        myMACdialog1.show();
                    }
                } // end actionitem
            ]
        }
    }
    Tab {
        title: qsTr("MAC 2")
        MainPage {
            id: tab2
            //mac: Wolmeupjs.macToStr(1);
            macTab: 2;
            actions: [
                // define the actions for tab here
                ActionItem {
                    title: qsTr("About")
                    imageSource: "asset:///images/help-about.png"
                    attachedObjects: [
                        SystemDialog {
                            id: myMACdialog2
                            title: "Wake on LAN"
                            body: myMACdialog1.body 
                            onFinished:{
                            } 
                        }
                    ]
                    onTriggered: {
                        // display help page
                        myMACdialog2.show();
                    }
                } // end actionitem
            ] // end actions
        }
    }
    Tab {
        title: qsTr("XBMC")
        imageSource: "asset:///images/remote_control.png"
        XbmcUI{
            id: xbmcUI
            actions: [
                // define the actions for tab here
                ActionItem {
                    title: qsTr("About")
                    imageSource: "asset:///images/help-about.png"
                    attachedObjects: [
                        SystemDialog {
                            id: myQmlDialog
                            title: "Simple XBMC remote"
                            body: "Provide IP address of XBMC box and remote events port (default 9777). Remote events must be enabled in XBMC settings as well as \"Receive events from other computers.\"\n\nCode by Miika Ahdesmaki 2013. miika.ahdesmaki@ovi.com"
                            onFinished:{
                            } 
                        }
                    ]
                    onTriggered: {
                        // display help page
                        myQmlDialog.show();
                    }
                } // end actionitem
            ] // end actions
        } // end XbmcUI
    } // end tab
    /*    
    Tab {
        title: qsTr("About")
        imageSource: "asset:///images/help-about.png"
        Page {
            Container {
                background: Color.DarkGray
                topPadding: 50
                leftPadding: 50
                rightPadding: 50

                ScrollView {
                    scrollViewProperties {
                        scrollMode: ScrollMode.Vertical
                    }
                    
                    TextArea {
                        editable: false
                        text: "Wake on LAN:\nProvide MAC address to wake up a computer in the same network. Two MAC separate MAC addresses can be configured into the two tabs. The MAC address gets sent to the broadcast address port 9 (IPv4). The computer must have Wake-on-LAN enabled in BIOS.\n\nXBMC remote: Provide IP address of XBMC box and remote events port (default 9777). Remote events must be enabled in XBMC settings as well as \"Receive events from other computers.\"\n\nCode by Miika Ahdesmaki 2013. miika.ahdesmaki@ovi.com"
                    }
                }
            }
        }
    }
     
    /*
    Tab {
        title: qsTr("MAC 3")
        Page {
            id: tab3
            actions: [
            // define the actions for tab here
            ActionItem {
                    title: qsTr("Raise")
                    onTriggered: {
                        // run the image animation
                        raiseAnimation.play();
                    }
                }
            ]
            Container {
                // define tab content here
            }
        }
    }
    */
    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        //console.log("TabbedPane - onCreationCompleted()")

        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
        // Initialize the database
        
        Storage.initialize();
        tab1.mac = Storage.getSetting("mac1");
        tab2.mac = Storage.getSetting("mac2");
        //tab3.mac = Storage.getSetting("mac3");
        Wolmeupjs.setMac(1, tab1.mac);
        Wolmeupjs.setMac(2, tab2.mac);
        //Wolmeupjs.setMac(3, tab3.mac);
    }
}
