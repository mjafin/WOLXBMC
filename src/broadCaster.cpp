#include <QtNetwork>
//#include <QUdpSocket>
#include <QString>
//#include <QDebug>
#include "broadCaster.h"

BroadCaster::BroadCaster(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
}

void BroadCaster::broadCastMAC(const QString &macaddress, const int portNumber)
{
    //QByteArray::fromHex
    //QByteArray::fill
    QByteArray myMAC = QByteArray::fromHex(macaddress.toAscii());

    QByteArray datagram;
    datagram.fill(0xff,6); // preclude by FFFFFF FFFFFF
    datagram.append(myMAC.repeated(16));
    udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, portNumber);
    //qDebug() << "MAC: " << datagram.toHex();
    //fprintf(stdout, "%s\n", datagram.toHex().data());
    //fflush(stdout); // 00:25:22:FE:6C:3B
}
