#ifndef BROADCASTER_H
#define BROADCASTER_H
#include <QObject>

class QUdpSocket;
class QString;
class BroadCaster : public QObject
{
    Q_OBJECT
public:
    BroadCaster(QObject *parent = 0); // : QObject(parent) { }
    Q_INVOKABLE void broadCastMAC(const QString &macaddress, const int portNumber = 9);
private:
    QUdpSocket *udpSocket;
};

#endif // BROADCASTER_H
