#include "snmpv1.h"
#include "snmpmessage.h"
#include "cvrt.h"
#include "session.h"

Snmpv1::Snmpv1(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort) : Snmpmessage(ipAddress, type, sendingPort, receivingPort)
{
    createSession();
}

Snmpv1::~Snmpv1()
{
    deleteSession();
}

void Snmpv1::sendMessage(const std::string str)
{
    constructMessage(str);
    session.sendMessage(senddedDatagram, receivedDatagram, sendingsize, receivingsize);
}

void Snmpv1::sendMessage(const int i, const std::string str)
{
    constructMessage(i, str);
    session.sendMessage(senddedDatagram, receivedDatagram, sendingsize, receivingsize);
}

void Snmpv1::sendMessage(const int32_t i, const std::string str)
{
    constructMessage(i, str);
    session.sendMessage(senddedDatagram, receivedDatagram, sendingsize, receivingsize);
}

void Snmpv1::sendMessage(const std::string str1, const std::string str2)
{
    constructMessage(str1, str2);
    session.sendMessage(senddedDatagram, receivedDatagram, sendingsize, receivingsize);
}

std::String Snmpv1::getError()
{

}

std::string Snmpv1::getVersion()
{
    return "SNMPv1";
}

void Snmpv1::setCommunityString(std::string communityString)
{
    this->communityString = communityString;
}

std::string Snmpv1::deciperErrorCode()
{

}
void Snmpv1::storeValue()
{

}

void constructMessage(const std::string str)
{
    char[] constructedoid = "";
    int16_t constructedoidlen = 0;
    cvrt::oidToRaw(oid, constructedoid, oid.length(), constructedoidlen);
}

void constructMessage(const int i, const std::string str)
{

}

void constructMessage(const int32_t i, const std::string str)
{

}

void constructMessage(const std::string str1, const std::string str2)
{

}

void SNMPSession::constructDatagram(QByteArray* datagram, QString valueParameter, QByteArray oid, int PDUfield)
{
    QByteArray communityStringarray = communityString.toLatin1();
    QByteArray value = valueParameter.toLatin1();
    int currentLength = 0;
    if(value != "")
    {
        bool isIPAddress = false;
        int dots = 0;

        for(int i=0;i<value.length();i++)
        {
            if(value.at(i) == '.')
                dots++;
        }

        if(dots == 3)
        {
            isIPAddress = true;
        }

        // include Value field

        if(isIPAddress)
        {
            QList<QByteArray> octets = value.split('.');
            for(int i=0;i<4;i++)
                datagram->push_back(octets[i].toInt());
            currentLength = 4;
            datagram->push_front(currentLength);
            datagram->push_front(0x40);
            currentLength += 2;
        } else
        {
            for(int i=0;i<value.size();i++)
            {
                datagram->push_back(value.at(i));
                currentLength++;
            }

            datagram->push_front(currentLength);
            datagram->push_front(0x04);
            currentLength += 2;
        }
    }
    else
    {
        // include Value field
        currentLength = 2;
        datagram->push_front((char)0x0);
        datagram->push_front(0x05);
    }

    convertOIDAccordingToBER(oid);

    // include Object Identifier field
    datagram->push_front(oid);
    currentLength += oid.size();
    datagram->push_front(oid.size());
    datagram->push_front(0x6);
    currentLength += 2;

    // include Varbind field
    datagram->push_front(currentLength);
    datagram->push_front(0x30);
    currentLength += 2;

    // include Varbind List field
    datagram->push_front(currentLength);
    datagram->push_front(0x30);
    currentLength += 2;

    // include Error Index field
    datagram->push_front((char)0x0);
    datagram->push_front(0x1);
    datagram->push_front(0x2);
    currentLength += 3;

    // include Error field
    datagram->push_front((char)0x0);
    datagram->push_front(0x1);
    datagram->push_front(0x2);
    currentLength += 3;

    // include Request ID
    datagram->push_front((rand() % 255 + 1));
    datagram->push_front(0x1);
    datagram->push_front(0x2);
    currentLength += 3;

    // include PDU field
    switch (PDUfield) {
    case 0:                                 //get request
        datagram->push_front(currentLength);
        datagram->push_front(0xA0);
        currentLength += 2;
        break;
    case 1:                                 //getnext request
        datagram->push_front(currentLength);
        datagram->push_front(0xA1);
        currentLength += 2;
        break;
    case 2:                                 //response
        datagram->push_front(currentLength);
        datagram->push_front(0xA2);
        currentLength += 2;
        break;
    case 3:                                 //set request
        datagram->push_front(currentLength);
        datagram->push_front(0xA3);
        currentLength += 2;
        break;
    case 4:                                 //trap pdu (snmpv1)
        datagram->push_front(currentLength);
        datagram->push_front(0xA4);
        currentLength += 2;
        break;
    case 5:                                 //getbulk request (snmpv2)
        datagram->push_front(currentLength);
        datagram->push_front(0xA5);
        currentLength += 2;
        break;
    case 6:                                 //informRequest
        datagram->push_front(currentLength);
        datagram->push_front(0xA6);
        currentLength += 2;
        break;
    case 7:                                 //trapv2 (snmpv2
        datagram->push_front(currentLength);
        datagram->push_front(0xA7);
        currentLength += 2;
        break;
    case 8:                                 //report
        datagram->push_front(currentLength);
        datagram->push_front(0xA8);
        currentLength += 2;
        break;
    default:
        break;
    }

    // include Community string
    datagram->push_front(communityStringarray);
    datagram->push_front(communityStringarray.size());
    datagram->push_front(0x04);
    currentLength += communityStringarray.size();
    currentLength += 2;

    // include SNMP version
    datagram->push_front((char)0x0);
    datagram->push_front(0x01);
    datagram->push_front(0x02);
    currentLength += 3;

    // finish the construction with the SNMP Message lenght and type code
    datagram->push_front(currentLength);
    datagram->push_front(0x30);
}
