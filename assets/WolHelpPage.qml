import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: wolHelpPage
    anchors.fill: parent
    Label{
        anchors.top: parent.top
        anchors.left: parent.left

        height: parent.height
        width: parent.width
        text: "This app provides Wake on LAN and XBMC remote functionality in one.<br><br>Three tabs for three MAC addresses are available. The addresses get stored in sqlite and reloaded at startup. IPV4 only currently. The wakeup packet gets broadcasted to port 9.<br><br>The XBMC remote automatically connects to the given IP address. When changing the IP, remember to press enter to register the new address and connect to XBMC. If connection is lost, open menu and reconnect.<br><br>By Miika Ahdesmaki (2012). Version 0.1.0. http://projects.developer.nokia.com/WOLMeUpXBMC"
    }
    tools: ToolBarLayout { ToolIcon { iconId: "toolbar-back"; onClicked: pageStack.pop();} }
}
