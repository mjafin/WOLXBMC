//
// Qt:ifyed version of http://xbmc.svn.sourceforge.net/viewvc/xbmc/trunk/tools/EventClients/lib/c%2B%2B/xbmcclient.h?revision=35744&view=markup
//
#ifndef XBMCQTCLIENT_H
#define XBMCQTCLIENT_H

//#include <QTime>
#include <QtNetwork>
#include <QString>
#include <QDebug>
#include <QFile>
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
// define constats
#define STD_PORT       9777

#define MS_ABSOLUTE    0x01
//#define MS_RELATIVE    0x02

#define BTN_USE_NAME   0x01
#define BTN_DOWN       0x02
#define BTN_UP         0x04
#define BTN_USE_AMOUNT 0x08
#define BTN_QUEUE      0x10
#define BTN_NO_REPEAT  0x20
#define BTN_VKEY       0x40
#define BTN_AXIS       0x80

#define PT_HELO         0x01
#define PT_BYE          0x02
#define PT_BUTTON       0x03
#define PT_MOUSE        0x04
#define PT_PING         0x05
#define PT_BROADCAST    0x06
#define PT_NOTIFICATION 0x07
#define PT_BLOB         0x08
#define PT_LOG          0x09
#define PT_ACTION       0x0A
#define PT_DEBUG        0xFF

#define ICON_NONE       0x00
#define ICON_JPEG       0x01
#define ICON_PNG        0x02
#define ICON_GIF        0x03

#define MAX_PACKET_SIZE  1024
#define HEADER_SIZE      32
#define MAX_PAYLOAD_SIZE (MAX_PACKET_SIZE - HEADER_SIZE)

#define MAJOR_VERSION 2
#define MINOR_VERSION 0

#define LOGDEBUG   0
#define LOGINFO    1
#define LOGNOTICE  2
#define LOGWARNING 3
#define LOGERROR   4
#define LOGSEVERE  5
#define LOGFATAL   6
#define LOGNONE    7

#define ACTION_EXECBUILTIN 0x01
#define ACTION_BUTTON      0x02
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
// test class that can be exposed to qml
class XbmcQtClient : public QObject
{
    Q_OBJECT
public:
    XbmcQtClient(QObject *parent = 0);
    Q_INVOKABLE void setServerSettings(const QString IPAddr, const int portToUse = STD_PORT); // UDP settings
    Q_INVOKABLE void sendKeyToXBMC(const char keyToSend); // send kbd keys
    Q_INVOKABLE void sendKeyToXBMC(const QByteArray xaction); // send kbd keys
    //Q_INVOKABLE void sendActionToXBMC(const QByteArray xaction); // send kbd keys
    Q_INVOKABLE void sendHeloToXBMC(); // send kbd keys
    Q_INVOKABLE void pingXBMC(); // send kbd keys
private:
    QHostAddress myAddress;
    int myPort;
};



//////////////////////////////////////////////////
//////////////////////////////////////////////////
// XBMC CLASSES, Qt:ifyed
//////////////////////////////////////////////////
//////////////////////////////////////////////////
class XBMCClientUtils
{
public:
    XBMCClientUtils() {}
    ~XBMCClientUtils() {}
    static unsigned int GetUniqueIdentifier()
    {
        static unsigned int id = qrand();
        return id;
    }
    // dummy functions
    static void Clean()
    {
    }
    static bool Initialize()
    {
        return true;
    }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////
class CPacket  : public QObject
{
    Q_OBJECT
/*   Base class that implements a single event packet.

     - Generic packet structure (maximum 1024 bytes per packet)
     - Header is 32 bytes long, so 992 bytes available for payload
     - large payloads can be split into multiple packets using H4 and H5
       H5 should contain total no. of packets in such a case
     - H6 contains length of P1, which is limited to 992 bytes
     - if H5 is 0 or 1, then H4 will be ignored (single packet msg)
     - H7 must be set to zeros for now

         -----------------------------
         | -H1 Signature ("XBMC")    | - 4  x CHAR                4B
         | -H2 Version (eg. 2.0)     | - 2  x UNSIGNED CHAR       2B
         | -H3 PacketType            | - 1  x UNSIGNED SHORT      2B
         | -H4 Sequence number       | - 1  x UNSIGNED LONG       4B
         | -H5 No. of packets in msg | - 1  x UNSIGNED LONG       4B
         | -H6 Payload size          | - 1  x UNSIGNED SHORT      2B
         | -H7 Client's unique token | - 1  x UNSIGNED LONG       4B
         | -H8 Reserved              | - 10 x UNSIGNED CHAR      10B
         |---------------------------|
         | -P1 payload               | -
         -----------------------------
*/
public:
    CPacket()
    {
        m_PacketType = 0;
        udpSocket = new QUdpSocket(this);
    }
    virtual ~CPacket()
    { }

    //bool Send(int Socket, CAddress &Addr, unsigned int UID = XBMCClientUtils::GetUniqueIdentifier())
    bool Send(int portNumber = STD_PORT, QHostAddress Addr = QHostAddress::LocalHost, unsigned int UID = XBMCClientUtils::GetUniqueIdentifier())
    {
        if (m_Payload.size() == 0)
            ConstructPayload();
        bool SendSuccessfull = true;
        int NbrOfPackages = (m_Payload.size() / MAX_PAYLOAD_SIZE) + 1;
        int Send = 0;
        int Sent = 0;
        int Left = m_Payload.size();
        for (int Package = 1; Package <= NbrOfPackages; Package++)
        {
            if (Left > MAX_PAYLOAD_SIZE)
            {
                Send = MAX_PAYLOAD_SIZE;
                Left -= Send;
            }
            else
            {
                Send = Left;
                Left = 0;
            }

            ConstructHeader(m_PacketType, NbrOfPackages, Package, Send, UID, m_Header);
            QByteArray datagram;
            datagram.clear();
            datagram.append(m_Header);
            //qDebug()<< " datagram " << datagram.toHex();
            datagram.append(m_Payload.mid(Sent,Send));
            //qDebug()<< " datagram " << datagram.toHex();

            // Send packet via UDP
            qint64 rtn = udpSocket->writeDatagram(datagram.data(), datagram.size(), Addr, portNumber);
            //qDebug() << "sent helo bytes" << rtn << " message " << datagram.toHex();
            if (rtn != (HEADER_SIZE + Send))
                SendSuccessfull = false;

            Sent += Send;
        }
        return SendSuccessfull;
    }
protected:
    QUdpSocket *udpSocket;
    //char            m_Header[HEADER_SIZE];
    QByteArray m_Header;
    unsigned short  m_PacketType;

    //std::vector<char> m_Payload;
    QByteArray m_Payload;

  static void ConstructHeader(int PacketType, int NumberOfPackets, int CurrentPacket, unsigned short PayloadSize, unsigned int UniqueToken, QByteArray &Header)//, char *Header)
  {
      Header.resize(HEADER_SIZE);
      Header.fill(0);
      Header.replace(0,4,"XBMC");
      //sprintf(Header, "XBMC");
      //for (int i = 4; i < HEADER_SIZE; i++)
      //  Header[i] = 0;
      Header[4]  = MAJOR_VERSION;
      Header[5]  = MINOR_VERSION;
      if (CurrentPacket == 1)
      {
          Header[6]  = ((PacketType & 0xff00) >> 8);
          Header[7]  =  (PacketType & 0x00ff);
      }
      else
      {
          Header[6]  = ((PT_BLOB & 0xff00) >> 8);
          Header[7]  =  (PT_BLOB & 0x00ff);
      }
      Header[8]  = ((CurrentPacket & 0xff000000) >> 24);
      Header[9]  = ((CurrentPacket & 0x00ff0000) >> 16);
      Header[10] = ((CurrentPacket & 0x0000ff00) >> 8);
      Header[11] =  (CurrentPacket & 0x000000ff);

      Header[12] = ((NumberOfPackets & 0xff000000) >> 24);
      Header[13] = ((NumberOfPackets & 0x00ff0000) >> 16);
      Header[14] = ((NumberOfPackets & 0x0000ff00) >> 8);
      Header[15] =  (NumberOfPackets & 0x000000ff);

      Header[16] = ((PayloadSize & 0xff00) >> 8);
      Header[17] =  (PayloadSize & 0x00ff);

      Header[18] = ((UniqueToken & 0xff000000) >> 24);
      Header[19] = ((UniqueToken & 0x00ff0000) >> 16);
      Header[20] = ((UniqueToken & 0x0000ff00) >> 8);
      Header[21] =  (UniqueToken & 0x000000ff);
  }

  virtual void ConstructPayload()
  { }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////

class CPacketHELO : public CPacket
{
    // ************************************************************************
    // * Payload format                                                       *
    // * %s -  device name (max 128 chars)                                    *
    // * %c -  icontype ( 0=>NOICON, 1=>JPEG , 2=>PNG , 3=>GIF )              *
    // * %s -  my port ( 0=>not listening )                                   *
    // * %d -  reserved1 ( 0 )                                                *
    // * %d -  reserved2 ( 0 )                                                *
    // * XX -  imagedata ( can span multiple packets )                        *
    // ************************************************************************
private:
    //std::vector<char> m_DeviceName;
    QByteArray m_DeviceName;
    unsigned short m_IconType;
    //char *m_IconData;
    QByteArray m_IconData;
    unsigned short m_IconSize;
public:
    virtual void ConstructPayload()
    {
        m_Payload.clear();

        //for (unsigned int i = 0; i < m_DeviceName.size(); i++)
        //    m_Payload.push_back(m_DeviceName[i]);
        m_Payload.append(m_DeviceName);
        //qDebug() << " m_Payload " <<m_Payload;
        m_Payload.append('\0');

        m_Payload.append((char)m_IconType);

        m_Payload.append((char)0);
        m_Payload.append('\0');

        for (int j = 0; j < 8; j++)
            m_Payload.append((char)0);

        //for (int ico = 0; ico < m_IconSize; ico++)
        //    m_Payload.push_back(m_IconData[ico]);
        m_Payload.append(m_IconData);
        //qDebug() << " m_Payload " <<m_Payload.toHex();
    }

    //CPacketHELO(const char *DevName, unsigned short IconType, const char *IconFile = NULL) : CPacket()
    CPacketHELO(const QByteArray &DevName, unsigned short IconType, const QByteArray &IconFile = "") : CPacket()
    {
        m_PacketType = PT_HELO;

        //unsigned int len = strlen(DevName);
        //unsigned int len = DevName.length();
        //for (unsigned int i = 0; i < len; i++)
        //    m_DeviceName.push_back(DevName[i]);
        m_DeviceName.append(DevName);
        m_IconType = IconType;

        if (IconType == ICON_NONE || IconFile.isEmpty())
        {
            //m_IconData = NULL;
            m_IconData = "";
            m_IconSize = 0;
            m_IconType = ICON_NONE;
            return;
        }

        QFile myFile(IconFile);
        if (myFile.open(QIODevice::ReadOnly)){
            m_IconData.clear();
            m_IconData = myFile.readAll();
            myFile.close();
            m_IconSize = m_IconData.size();
            //qDebug()<<"Icon size "<<m_IconData.size();
            //fprintf(stdout, "Icon size %d\n", m_IconData.size());
            //fflush(stdout); // 00:25:22:FE:6C:3B
        }else{
            m_IconType = ICON_NONE;
            m_IconSize = 0;
        }
    }

    virtual ~CPacketHELO()
    {
        m_DeviceName.clear();
        m_IconData.clear();
        //if (m_IconData)
        //    delete m_IconData;
        //    free(m_IconData);
    }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////
class CPacketBUTTON : public CPacket
{
    // ************************************************************************
    // * Payload format                                                       *
    // * %i - button code                                                     *
    // * %i - flags 0x01 => use button map/name instead of code               *
    // *            0x02 => btn down                                          *
    // *            0x04 => btn up                                            *
    // *            0x08 => use amount                                        *
    // *            0x10 => queue event                                       *
    // *            0x20 => do not repeat                                     *
    // *            0x40 => virtual key                                       *
    // *            0x40 => axis key                                          *
    // * %i - amount ( 0 => 65k maps to -1 => 1 )                             *
    // * %s - device map (case sensitive and required if flags & 0x01)        *
    // *      "KB" - Standard keyboard map                                    *
    // *      "XG" - Xbox Gamepad                                             *
    // *      "R1" - Xbox Remote                                              *
    // *      "R2" - Xbox Universal Remote                                    *
    // *      "LI:devicename" -  valid LIRC device map where 'devicename'     *
    // *                         is the actual name of the LIRC device        *
    // *      "JS<num>:joyname" -  valid Joystick device map where            *
    // *                           'joyname'  is the name specified in        *
    // *                           the keymap. JS only supports button code   *
    // *                           and not button name currently (!0x01).     *
    // * %s - button name (required if flags & 0x01)                          *
    // ************************************************************************
private:
    //std::vector<char> m_DeviceMap;
    QByteArray m_DeviceMap;
    //std::vector<char> m_Button;
    QByteArray m_Button;
    unsigned short m_ButtonCode;
    unsigned short m_Amount;
    unsigned short m_Flags;
public:
    virtual void ConstructPayload()
    {
        m_Payload.clear();

        if (m_Button.size() != 0)
        {
            if (!(m_Flags & BTN_USE_NAME)) // If the BTN_USE_NAME isn't flagged for some reason
                m_Flags |= BTN_USE_NAME;
            m_ButtonCode = 0;
        }
        else
            m_Button.clear();

        if (m_Amount > 0)
        {
            if (!(m_Flags & BTN_USE_AMOUNT))
                m_Flags |= BTN_USE_AMOUNT;
        }
        if (!((m_Flags & BTN_DOWN) || (m_Flags & BTN_UP))) //If none of them are tagged.
            m_Flags |= BTN_DOWN;

        m_Payload.append((char)((m_ButtonCode & 0xff00) >> 8));
        m_Payload.append((char) (m_ButtonCode & 0x00ff));

        m_Payload.append((char)((m_Flags & 0xff00) >> 8) );
        m_Payload.append((char) (m_Flags & 0x00ff));

        m_Payload.append((char)((m_Amount & 0xff00) >> 8) );
        m_Payload.append((char) (m_Amount & 0x00ff));


        //for (unsigned int i = 0; i < m_DeviceMap.size(); i++)
        //    m_Payload.push_back(m_DeviceMap[i]);
        m_Payload.append(m_DeviceMap);

        m_Payload.append('\0');

        //for (unsigned int i = 0; i < m_Button.size(); i++)
        //    m_Payload.push_back(m_Button[i]);
        m_Payload.append(m_Button);

        m_Payload.append('\0');
    }

    //CPacketBUTTON(const char *Button, const char *DeviceMap, unsigned short Flags, unsigned short Amount = 0) : CPacket()
    CPacketBUTTON(const QByteArray &Button, const QByteArray &DeviceMap, unsigned short Flags, unsigned short Amount = 0) : CPacket()
    {
        m_PacketType = PT_BUTTON;
        m_Flags      = Flags;
        m_ButtonCode = 0;
        m_Amount     = Amount;

        //unsigned int len = strlen(DeviceMap);
        //for (unsigned int i = 0; i < len; i++)
        //    m_DeviceMap.push_back(DeviceMap[i]);
        m_DeviceMap.append(DeviceMap);

        //len = strlen(Button);
        //for (unsigned int i = 0; i < len; i++)
        //    m_Button.push_back(Button[i]);
        m_Button.append(Button);
    }

    //CPacketBUTTON(unsigned short ButtonCode, const char *DeviceMap, unsigned short Flags, unsigned short Amount = 0) : CPacket()
    CPacketBUTTON(unsigned short ButtonCode, const QByteArray &DeviceMap, unsigned short Flags, unsigned short Amount = 0) : CPacket()
    {
        m_PacketType = PT_BUTTON;
        m_Flags      = Flags;
        m_ButtonCode = ButtonCode;
        m_Amount     = Amount;

        //unsigned int len = strlen(DeviceMap);
        //for (unsigned int i = 0; i < len; i++)
        //    m_DeviceMap.push_back(DeviceMap[i]);
        m_DeviceMap.append(DeviceMap);
    }

    CPacketBUTTON(unsigned short ButtonCode, unsigned short Flags, unsigned short Amount = 0) : CPacket()
    {
        m_PacketType = PT_BUTTON;
        m_Flags      = Flags;
        m_ButtonCode = ButtonCode;
        m_Amount     = Amount;
    }

    // Used to send a release event
    CPacketBUTTON() : CPacket()
    {
        m_PacketType = PT_BUTTON;
        m_Flags = BTN_UP;
        m_Amount = 0;
        m_ButtonCode = 0;
    }

    virtual ~CPacketBUTTON()
    {
        m_DeviceMap.clear();
        m_Button.clear();
    }

    inline unsigned short GetFlags() { return m_Flags; }
    inline unsigned short GetButtonCode() { return m_ButtonCode; }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////

class CPacketPING : public CPacket
{
    // ************************************************************************
    // * no payload                                                           *
    // ************************************************************************
public:
    CPacketPING() : CPacket()
    {
        m_PacketType = PT_PING;
    }
    virtual ~CPacketPING()
    { }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////

class CPacketBYE : public CPacket
{
    // ************************************************************************
    // * no payload                                                           *
    // ************************************************************************
public:
    CPacketBYE() : CPacket()
    {
        m_PacketType = PT_BYE;
    }
    virtual ~CPacketBYE()
    { }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////
class CPacketNOTIFICATION : public CPacket
{
    // ************************************************************************
    // * Payload format:                                                      *
    // * %s - caption                                                         *
    // * %s - message                                                         *
    // * %c - icontype ( 0=>NOICON, 1=>JPEG , 2=>PNG , 3=>GIF )               *
    // * %d - reserved ( 0 )                                                  *
    // * XX - imagedata ( can span multiple packets )                         *
    // ************************************************************************
private:
    //std::vector<char> m_Title;
    //std::vector<char> m_Message;
    QByteArray m_Title;
    QByteArray m_Message;
    unsigned short m_IconType;
    //char *m_IconData;
    QByteArray m_IconData;
    unsigned short m_IconSize;
public:
    virtual void ConstructPayload()
    {
        m_Payload.clear();

        //for (unsigned int i = 0; i < m_Title.size(); i++)
        //    m_Payload.push_back(m_Title[i]);
        m_Payload.append(m_Title);
        m_Payload.append('\0');

        //for (unsigned int i = 0; i < m_Message.size(); i++)
        //    m_Payload.push_back(m_Message[i]);
        m_Payload.append(m_Message);

        m_Payload.append('\0');

        m_Payload.append((char) m_IconType);

        for (int i = 0; i < 4; i++)
            m_Payload.append((char) 0);

        for (int ico = 0; ico < m_IconSize; ico++)
            m_Payload.append(m_IconData[ico]);
    }

    //CPacketNOTIFICATION(const char *Title, const char *Message, unsigned short IconType, const char *IconFile = NULL) : CPacket()
    CPacketNOTIFICATION(const QByteArray &Title, const QByteArray &Message, unsigned short IconType, const QString &IconFile = "") : CPacket()
    {
        m_PacketType = PT_NOTIFICATION;
        m_IconData = "";
        m_IconSize = 0;
        /*
        unsigned int len = 0;
        if (Title != NULL)
        {
            len = strlen(Title);
            for (unsigned int i = 0; i < len; i++)
                m_Title.push_back(Title[i]);
        }

        if (Message != NULL)
        {
            len = strlen(Message);
            for (unsigned int i = 0; i < len; i++)
                m_Message.push_back(Message[i]);
        }
        */
        m_Title.append(Title);
        m_Message.append(Message);
        m_IconType = IconType;

        //if (IconType == ICON_NONE || IconFile == NULL)
        if (IconType == ICON_NONE || IconFile.isEmpty())
            return;

        QFile myFile(IconFile);
        if (myFile.open(QIODevice::ReadOnly)){
            m_IconData.clear();
            m_IconData = myFile.readAll();
            myFile.close();
            m_IconSize = m_IconData.size();
        }{
            m_IconType = ICON_NONE;
            m_IconSize = 0;
        }
    }

    virtual ~CPacketNOTIFICATION()
    {
        m_Title.clear();
        m_Message.clear();
        m_IconData.clear();
        //if (m_IconData)
            //free(m_IconData);
    }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////

class CPacketMOUSE : public CPacket
{
    // ************************************************************************
    // * Payload format                                                       *
    // * %c - flags                                                           *
    // *    - 0x01 absolute position                                          *
    // * %i - mousex (0-65535 => maps to screen width)                        *
    // * %i - mousey (0-65535 => maps to screen height)                       *
    // ************************************************************************
private:
  unsigned short m_X;
  unsigned short m_Y;
  unsigned char  m_Flag;
public:
  CPacketMOUSE(int X, int Y, unsigned char Flag = MS_ABSOLUTE)
  {
    m_PacketType = PT_MOUSE;
    m_Flag = Flag;
    m_X = X;
    m_Y = Y;
  }

  virtual void ConstructPayload()
  {
    m_Payload.clear();

    m_Payload.append((char) m_Flag);

    m_Payload.append((char) ((m_X & 0xff00) >> 8));
    m_Payload.append((char) (m_X & 0x00ff));

    m_Payload.append((char) ((m_Y & 0xff00) >> 8));
    m_Payload.append((char) (m_Y & 0x00ff));
  }

  virtual ~CPacketMOUSE()
  { }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////

class CPacketLOG : public CPacket
{
    // ************************************************************************
    // * Payload format                                                       *
    // * %c - log type                                                        *
    // * %s - message                                                         *
    // ************************************************************************
private:
  //std::vector<char> m_Message;
    QByteArray m_Message;
    unsigned char  m_LogLevel;
    bool m_AutoPrintf;
public:
    //CPacketLOG(int LogLevel, const char *Message, bool AutoPrintf = true)
    CPacketLOG(int LogLevel, const QByteArray &Message, bool AutoPrintf = true)
    {
        m_PacketType = PT_LOG;
        /*
        unsigned int len = strlen(Message);
        for (unsigned int i = 0; i < len; i++)
            m_Message.push_back(Message[i]);
        */
        m_Message.append(Message);
        m_LogLevel = LogLevel;
        m_AutoPrintf = AutoPrintf;
    }

    virtual void ConstructPayload()
    {
        m_Payload.clear();

        m_Payload.append((char) (m_LogLevel & 0x00ff) );

        if (m_AutoPrintf)
        {
            //char* str=&m_Message[0];
            //printf("%s\n", str);
            qDebug() << m_Message;
        }
        //for (unsigned int i = 0; i < m_Message.size(); i++)
        //    m_Payload.push_back(m_Message[i]);
        m_Payload.append(m_Message);

        m_Payload.append('\0');
    }

    virtual ~CPacketLOG()
    { }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////

class CPacketACTION : public CPacket
{
    // ************************************************************************
    // * Payload format                                                       *
    // * %c - action type                                                     *
    // * %s - action message                                                  *
    // ************************************************************************
private:
  unsigned char     m_ActionType;
  //std::vector<char> m_Action;
  QByteArray m_Action;
public:
  //CPacketACTION(const char *Action, unsigned char ActionType = ACTION_EXECBUILTIN)
  CPacketACTION(const QByteArray &Action, unsigned char ActionType = ACTION_EXECBUILTIN)
  {
    m_PacketType = PT_ACTION;

    m_ActionType = ActionType;
    //unsigned int len = strlen(Action);
    //for (unsigned int i = 0; i < len; i++)
    //  m_Action.push_back(Action[i]);
    m_Action.append(Action);
  }

  virtual void ConstructPayload()
  {
    m_Payload.clear();

    m_Payload.append(m_ActionType);
    //for (unsigned int i = 0; i < m_Action.size(); i++)
    //  m_Payload.push_back(m_Action[i]);
    m_Payload.append(m_Action);

    m_Payload.append('\0');
  }

  virtual ~CPacketACTION()
  { }
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////

#endif // XBMCQTCLIENT_H
