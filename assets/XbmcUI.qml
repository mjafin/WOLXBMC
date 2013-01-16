import bb.cascades 1.0
import "storage.js" as Storage

Page {
    property string iPaddress: "127.0.0.1"
    property string xbmcport: "9777"
    signal setXBMCSettings
    
    onCreationCompleted:{
        Storage.initialize();
    }
    onSetXBMCSettings:{
        //console.log("debug xbmcui reached");
        iPaddress = Storage.getSetting("IP");
        xbmcport = Storage.getSetting("port");
        if (iPaddress === "000000000000" || xbmcport === "000000000000"){ // "000000000000" is a default value given for an unexisting entry in storage.js
            Storage.setSetting("IP","127.0.0.1"); Storage.setSetting("port","9777");
            iPaddress = "127.0.0.1"; xbmcport = "9777";
        }
        XbmcQtClient.setServerSettings(iPaddress, parseInt(xbmcport,10));
        XbmcQtClient.sendHeloToXBMC();
        //console.log("xbmcui "+iPaddress+" ");
    }
    
    Container {
        id: xbmcmaincon
        background: Color.DarkGray//Color
        layout: StackLayout {
        }
        //
        // TOP CONTAINER
        //
        Container {
            id: topcon
            horizontalAlignment: HorizontalAlignment.Center
            topPadding: 15
            bottomPadding: 50
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Container {
                layout: StackLayout {
                }
                Label {
                    text: "XBMC IP"
                }
                TextField {
                    id: ipInputField
                    text: iPaddress
                    inputMode: TextFieldInputMode.NumbersAndPunctuation
                    input {
                        onSubmitted:{
                            // check for validity of IP address (ugly regexp)
                            var patt1 = /^(([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))\.){3}([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))$/i;
                            if (ipInputField.text.match(patt1)){
                                iPaddress = ipInputField.text; Storage.setSetting("IP",iPaddress); XbmcQtClient.setServerSettings(iPaddress, parseInt(xbmcport,10)); XbmcQtClient.sendHeloToXBMC();
                            }else{
                                ipInputField.text = iPaddress;
                            }
                        }
                    }
                }
            }
            Container {
                layout: StackLayout {
                }
                Label {
                    text: "XBMC event port"
                }
                TextField {
                    id: portInputField
                    text: xbmcport
                    inputMode: TextFieldInputMode.NumbersAndPunctuation
                    input {
                        onSubmitted:{
                            xbmcport = portInputField.text; Storage.setSetting("port",xbmcport); XbmcQtClient.setServerSettings(iPaddress, parseInt(xbmcport,10)); XbmcQtClient.sendHeloToXBMC();
                        }
                    }
                }
            }             
        }
        
        //
        // MIDDLE CONTAINER
        //
        Container {
            id: middlecon
            layout: StackLayout {
            }
            Container { //Back, Context, Fullscreen
                horizontalAlignment: HorizontalAlignment.Center
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Button {
                    text: "Back"
                    imageSource: "asset:///images/edit-undo.png"
                    onClicked: {XbmcQtClient.sendKeyToXBMC("backspace");}
                }
                Button {
                    text: "Contxt"
                    imageSource: "asset:///images/view-list-text.png"
                    onClicked: {XbmcQtClient.sendKeyToXBMC("C");}
                }
                Button {
                    text: "Full"
                    imageSource: "asset:///images/view-fullscreen.png"
                    onClicked: {XbmcQtClient.sendKeyToXBMC("tab");}
                }
            }
            Container {//up, down, left, right, select/OSD
                horizontalAlignment: HorizontalAlignment.Center
                topPadding: 40
                bottomPadding: 40
                Button {
                    horizontalAlignment: HorizontalAlignment.Center
                    imageSource: "asset:///images/button_up.png"
                    onClicked: {XbmcQtClient.sendKeyToXBMC("up");}
                }
                Container {
                    topMargin: 30
                    bottomMargin: 30
                    horizontalAlignment: HorizontalAlignment.Center
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Button {
                        imageSource: "asset:///images/button_left.png"
                        maxWidth: 150
                        onClicked: {XbmcQtClient.sendKeyToXBMC("left");}
                    }
                    Button {
                        text: "Select/OSD"
                        imageSource: "asset:///images/key_enter.png"
                        onClicked: {XbmcQtClient.sendKeyToXBMC("return");}
                    }
                    Button {
                        imageSource: "asset:///images/button_right.png"
                        maxWidth: 150
                        onClicked: {XbmcQtClient.sendKeyToXBMC("right");}
                    }
                }
                Button {
                    horizontalAlignment: HorizontalAlignment.Center
                    imageSource: "asset:///images/button_down.png"
                    onClicked: {XbmcQtClient.sendKeyToXBMC("down");}
                }
            }
            Container { // Vol +-
                horizontalAlignment: HorizontalAlignment.Center
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Button {
                    text: "Vol-"
                    imageSource: "asset:///images/vol_down.png"
                    onClicked: {XbmcQtClient.sendKeyToXBMC("minus");}
                }
                Button {
                    text: "Vol+"
                    imageSource: "asset:///images/vol_up.png"
                    onClicked: {XbmcQtClient.sendKeyToXBMC("plus");}
                }
            }
        }
        
        //
        // BOTTOM CONTAINER
        //
        Container {
            id: bottomcon
            horizontalAlignment: HorizontalAlignment.Center
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            topPadding: 50

            Button {
                //maxWidth: 20
                imageSource: "asset:///images/media-seek-backward.png"
                onClicked: {XbmcQtClient.sendKeyToXBMC("r");}
            }
            Button {
                //maxWidth: 20
                imageSource: "asset:///images/media-playback-stop.png"
                onClicked: {XbmcQtClient.sendKeyToXBMC("X");}
            }
            Button {
                //maxWidth: 20
                imageSource: "asset:///images/media-playback-pause.png"
                onClicked: {XbmcQtClient.sendKeyToXBMC("play_pause");}
            }
            Button {
                //maxWidth: 20
                imageSource: "asset:///images/media-seek-forward.png"
                onClicked: {XbmcQtClient.sendKeyToXBMC("f");}
            }
        }
    }
}

/*
    // set some settings
    Timer { // using a timer instead of Component.onCompleted as XbmcQtClient might not have been exposed yet..?
        id: settingTimer
        interval: 100; running: false; repeat: false;
        onTriggered: {
            iPaddress = Storage.getSetting("IP");
            xbmcport = Storage.getSetting("port");
            if (iPaddress === "000000000000" || xbmcport === "000000000000"){ // "000000000000" is a default value given for an unexisting entry in storage.js
                Storage.setSetting("IP","127.0.0.1"); Storage.setSetting("port","9777");
                iPaddress = "127.0.0.1"; xbmcport = "9777";
            }
            XbmcQtClient.setServerSettings(iPaddress, parseInt(xbmcport,10));
            XbmcQtClient.sendHeloToXBMC();
        }
    }
*/