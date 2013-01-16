#include "xbmcQtClient.h"
#include <QtNetwork>
#include <QString>
//#include <QDebug>
XbmcQtClient::XbmcQtClient(QObject *parent) : QObject(parent)
{
    myAddress = QHostAddress::LocalHost;
    myPort = 9777;
}

void XbmcQtClient::setServerSettings(const QString IPAddr, const int portToUse)
{
    myAddress.setAddress(IPAddr);
    myPort = portToUse;
    //fprintf(stdout, "%s %d\n", IPAddr.toAscii().constData(),portToUse);
    //fflush(stdout); // 00:25:22:FE:6C:3B
}
void XbmcQtClient::sendKeyToXBMC(const char keyToSend){
    // THIS function takes as input char codes (e.g. 'S')
    // CURRENTLY THIS FUNCTION DOESN'T DO WHAT IT'S SUPPOSED TO FOR SOME REASON
    //
    //qDebug() << "key to send: " << (int) keyToSend;
    //qDebug() << "IP: " << myAddress.toString() << " port " << myPort;
    CPacketBUTTON(keyToSend, BTN_DOWN | BTN_QUEUE).Send(myPort, myAddress);
    CPacketBUTTON(keyToSend, BTN_UP | BTN_QUEUE).Send(myPort, myAddress);
}
void XbmcQtClient::sendKeyToXBMC(const QByteArray xaction){
    // This function accepts strings, such as "down", "enter" etc.
    //
    //qDebug() << "command to send: " << xaction;
    CPacketBUTTON(xaction, "KB", BTN_USE_NAME | BTN_DOWN | BTN_QUEUE).Send(myPort, myAddress);
    CPacketBUTTON(xaction, "KB", BTN_USE_NAME | BTN_UP | BTN_QUEUE).Send(myPort, myAddress);
}

//void XbmcQtClient::sendActionToXBMC(const QByteArray xaction){;} // send kbd keys

void XbmcQtClient::sendHeloToXBMC(){
    // this package is sent to greet XBMC
    //CPacketHELO(QByteArray("WOLMeXBMC"), 0).Send(myPort, myAddress);
    //CPacketHELO(QByteArray("WOLMeXBMC"), ICON_PNG, ":/WOLMeUp80.png").Send(myPort, myAddress);
	QString appFolder(QDir::currentPath());
	QString fileName = appFolder + "/app/native/assets//images/WOLMeUp80.png";
	CPacketHELO(QByteArray("WOLMeXBMC"), ICON_PNG, fileName.toAscii()).Send(myPort, myAddress);
}

void XbmcQtClient::pingXBMC(){
    // this function pings xbmc
    CPacketPING().Send(myPort, myAddress);
}
