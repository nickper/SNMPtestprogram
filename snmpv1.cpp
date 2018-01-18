#include "snmpv1.h"
#include "snmpmessage.h"
#include "cvrt.h"
#include "session.h"

#define INT 0x02
#define BIT 0x03
#define STR 0x04
#define END 0x05
#define OID 0x06
#define SEQ 0x30
#define IP  0x40
#define C32 0x41
#define GAU 0x42
#define TIME 0x43
#define OPA  0x44
#define NSAP 0x45
#define C64  0x46
#define U32  0x47

Snmpv1::Snmpv1(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort) : Snmpmessage(ipAddress, type, sendingPort, receivingPort)
{
    createSession();
}

Snmpv1::~Snmpv1()
{
    deleteSession();
}

void Snmpv1::sendMessage(std::string &value, enumvalue &valuetype)
{
    this->constructMessage(value, valuetype);
    this->session->sendMessage(sendedDatagram, receivedDatagram, sendingsize, receivingsize);
}

enumerror Snmpv1::getError()
{
    return this->error;
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
    switch (error) {
        case NOERROR: std::cout << "no error" << std::endl; return "no error"; break;
        case TOOBIG: std::cout << "error: TooBig" << std::endl;return "TooBig"; break;
        case NOSUCHNAME: std::cout << "error: NoSuchName" << std::endl; return "NoSuchName"; break;
        case BADVALUE: std::cout << "error: Badvalue" << std::endl; return "Badvalue"; break;
        case READONLY: std::cout << "error: ReadOnly" << std::endl;return "ReadOnly"; break;
        case GENERR: std::cout << "error: GenErr" << std::endl; return "GenErr"; break;
        case NOACCESS: std::cout << "error: NoAccess" << std::endl; return "NoAccess"; break;
        case WRONGTYPE: std::cout << "error: WrongType" << std::endl; return "WrongType"; break;
        case WRONGLENGHT: std::cout << "error: WrongLength" << std::endl; return "WrongLength"; break;
        case WRONGENCODING: std::cout << "error: WrongEncoding" << std::endl; return "WrongEncoding"; break;
        case WRONGVALUE:std::cout << "error: WrongValue" << std::endl; return "WrongValue"; break;
        case NOCREATION:std::cout << "error: NoCreation" << std::endl; return "NoCreation"; break;
        case INCOSISTENTVALUE:std::cout << "error: InconsistentValue" << std::endl; return  "InconsistentValue"; break;
        case RESOURCEUNAVAILABLE:std::cout << "error: ResourceUnavailable" << std::endl; return "ResourceUnavailable"; break;
        case COMMITFAULED:std::cout << "error: CommitFauled" << std::endl; return "CommitFauled"; break;
        case UNDOFAILED:std::cout << "error: UndoFailed" << std::endl; return "UndoFailed"; break;
        case AUTHORIZATIONERROR:std::cout << "error: AuthorizationError" << std::endl; return "AuthorizationError"; break;
        case NOTWRITABLE:std::cout << "error: NotWritable" << std::endl; return "NotWritable"; break;
        case INCONSISTENDNAME:std::cout << "error: InconsistentName" << std::endl; return "InconsistentName"; break;
        default:std::cout << "unknown error occur" << std::endl;return "unknown";break;
        }
}
void Snmpv1::storeValue()
{
        std::string communityStringreceived = "";
        std::string oid = "1.3";

        uint8_t messageType =0;
        uint8_t seqID =0;
        int16_t oidLenght = 0;
        uint8_t version = 255;
        uint8_t errorCode = 0;
        int16_t receivedoidlen = 0;

        uint8_t lenght = receivedDatagram[1];
        uint64_t verbindFieldLenght = 0;
        uint64_t verbindListLenght = 0;
        uint64_t valueLength = 0;
        uint64_t longlenght = 0;
        uint64_t pduLenght = 0;
        uint64_t messageIndex = 1;

        uint j, i;

        // ---------- retreive total message length ------------
        if(lenght >= 127)
        {
            j = 0;
            for(int i = lenght-128 -1; i >= 0 ; i--)
            {
                longlenght = (receivedDatagram[i+2] << ((j)*8)) | longlenght;
                j++;
            }
            messageIndex += j;
        }
        else
        {
            longlenght = lenght;
        }
        messageIndex += 3;

        if(longlenght == 0x36)
        {

            int dummy = 0;
        }

        // ---------- retreive SNMP version ------------
        version = receivedDatagram[messageIndex];
        messageIndex += 2;

        // ---------- retreive community string ------------
        for(i = 1; i <= (unsigned)receivedDatagram[messageIndex]; i++)
        {
            communityStringreceived += receivedDatagram[messageIndex+i];
        }
        messageIndex += i;

        // ---------- retreive SNMP message type ------------
        messageType = receivedDatagram[messageIndex];
        messageIndex += 1;

        // ---------- retreive total PDU length ------------
        if((unsigned)receivedDatagram[messageIndex] >= 127)
        {
            j = 0;
            for(uint8_t i = (unsigned)receivedDatagram[messageIndex]-128; i > 0 ; i--)
            {
                pduLenght = (receivedDatagram[messageIndex+i]) << ((j)*8) | pduLenght;
                j++;
            }
            messageIndex += j;
        }
        else
        {
            pduLenght = receivedDatagram[messageIndex];
        }
        messageIndex +=3;

        // ---------- retreive sequence ID ------------
        seqID = receivedDatagram[messageIndex];
        messageIndex +=3;

        // ---------- return error code if its occurs ------------
        if(receivedDatagram[messageIndex] != 0)
        {
            errorCode = receivedDatagram[messageIndex];
        }
        messageIndex +=5;

        // ---------- retreive total verbindfield length ------------
        if((unsigned)receivedDatagram[messageIndex] >= 127)
        {
            j = 0;
            for(uint8_t i = (unsigned)receivedDatagram[messageIndex]-128; i > 0 ; i--)
            {
                verbindFieldLenght = (receivedDatagram[messageIndex+i]) << ((j)*8) | verbindFieldLenght;
                j++;
            }
            messageIndex += j;
        }
        else
        {
            verbindFieldLenght = receivedDatagram[messageIndex];
        }
        messageIndex +=2;

        // ---------- retreive total verbindlist length ------------
        if((unsigned)receivedDatagram[messageIndex] >= 127)
        {
            j = 0;
            for(uint8_t i = (unsigned)receivedDatagram[messageIndex]-128; i > 0 ; i--)
            {
                verbindListLenght = (receivedDatagram[messageIndex+i]) << ((j)*8) | verbindListLenght;
                j++;
            }
            messageIndex += j;
        }
        else
        {
            verbindListLenght = receivedDatagram[messageIndex];
        }
        messageIndex +=2;

        // ---------- retreive OID  ------------
        j = 1;
        oidLenght = receivedDatagram[messageIndex];
        char tempoidchar[oidLenght];
        for(int i = 1; i < oidLenght; i++)
        {
            tempoidchar[i-1] = receivedDatagram[messageIndex + i];
        }

        cvrt::rawToOid(tempoidchar, oid, oidLenght, receivedoidlen);
        messageIndex += oidLenght + 1;
//        for(i=2; i <= oidLenght ; i++)
//        {
//            oid += ".";
//            if((unsigned)receivedDatagram[messageIndex+i] <= 127)
//            {

//                oid += cvrt::
//                        QString::number((unsigned)receivedDatagram[messageIndex+i]);
//            }
//            else
//            {
//                uint8_t index = 0;
//                uint64_t longoid = 0;
//                uint8_t temp = receivedDatagram[messageIndex +i];
//                temp &= ~128;
//                longoid = longoid << ( index * 7) | temp;
//                while ((uint8_t)receivedDatagram[messageIndex +i + index] > 127)
//                {
//                    index++;
//                    temp = receivedDatagram[messageIndex +i + index];
//                    temp &= ~128;
//                    longoid = longoid << 7 | temp;
//                    j++;
//                }
//                oid += QString::number(longoid,10);
//                i += index;
//            }
//            j++;
//        }
//        messageIndex +=j;

        // ---------- retreive value ------------
        messageIndex +=2;
        switch ((unsigned)receivedDatagram[messageIndex-1]) {
        case INT:
        {
            valuetype = "INTEGER";
            uint tempvalue = 0;
            valueLength = receivedDatagram[messageIndex];
            //messageIndex +=1;
            j=0;
            for(int i = valueLength; i > 0 ; i--)
            {
                value = (receivedDatagram[messageIndex+i]) << ((j)*8) | tempvalue;
                j++;
            }
            value = tempvalue;
            messageIndex += j;
            break;
        }
        case BIT:
        {
            valuetype = "BIT STRING";
            value = "BITSTRING";
            break;
        }
        case STR:
        {
            valuetype = "OCTET STRING";
            std::string tempvalue = "";
            if((uint8_t)receivedDatagram[messageIndex] >= 127)
            {
                j = 0;
                for(uint8_t i = ((uint8_t)receivedDatagram[messageIndex])-128; i > 0  ; i--)
                {
                    valueLength = (uint8_t)(receivedDatagram[messageIndex+i]) << ((j)*8) | valueLength;
                    j++;
                }
            }
            else
            {
                valueLength = receivedDatagram[messageIndex];
            }
            messageIndex +=1;
            for(i = 1; i <= valueLength; i++)
            {
                tempvalue += receivedDatagram[messageIndex];
                messageIndex +=1;
            }
            value = tempvalue;
            break;
        }
        case END:
        {
            valuetype = "NULL";
            if(messageIndex != longlenght)
            {
                value = "message bevat meerdere value's of oid's";
            }
            else
            {
                value = "message bevat verder geen value";
            }
            break;
        }
        case OID:
        {
            valuetype = "OBJECT IDENTIFIER";
            value = "OID";
            break;
        }
        case SEQ:
        {

            valuetype = "SEQUENCE";
            value = "SEQENTIE";
            break;
        }
        case IP:
        {
            valuetype = "IpAddress";
            value = "IP address";
            break;
        }
        case C32:
        {
            valuetype = "32 Bits Counter";
            value = "32 Bits Counter";
            break;
        }
        case GAU:
        {
            valuetype = "32 Bits Gauge";
            value = "32 Bits Gauge";
            break;
        }
        case TIME:
        {
            valuetype = "TimeTicks";
            uint32_t tempvalue = 0;
            valueLength = receivedDatagram[messageIndex];
            //messageIndex +=1;
            j=0;
            for(int i = valueLength; i > 0 ; i--)
            {
                tempvalue =  ((unsigned)receivedDatagram[messageIndex+i]) << ((j)*8) | tempvalue;
                j++;
            }
            value = std::to_string(tempvalue);
            break;
        }
        case OPA:
        {
            valuetype = "Opaque";
            value = "Opaque";
            break;
        }
        case NSAP:
        {
            valuetype = "NsapAdress";
            value = "NsapAdress";
            break;
        }
        case C64:
        {
            valuetype = "64 Bits Counter";
            value = "64 Bits Counter";
            break;
        }
        case U32:
        {
            valuetype = "Unsigned Int 32";
            value = "Unsigned Int 32";
            break;
        }
        default:
            valuetype = "Default";
            value = "Default";
            break;
        }
}

std::deque Snmpv1::constructMessage(std::string &value, enumvalue &valuetype)
{
    std::deque<char> message;
    uint16_t verbindfieldlen = 0;
    uint16_t verbindlistlen = 0;
    uint16_t pdulen = 0;
    uint16_t messagelen = 0;

    std::string constructedoid = "";\

    switch (valuetype) {
    case INTEGER:
        uint32_t tempvalue = std::stoi(value);
        message.push_front(tempvalue);
        if(tempvalue < 256)
            message.push_front(0x01);
        else if(tempvalue < 65536)
            message.push_front(0x02);
        else if(tempvalue < 16777216)
            message.push_front(0x03);
        else
            message.push_front(0x04);
        message.push_front(INTEGER);
        break;
    case BITSTRING:
                                        //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    case OCTETSTRING:
        message.push_front(value);
        message.push_front(cvrt::convertIntAccordingToBER(value.size()));
        message.push_front(OCTETSTRING);
        break;
    case EMPTY:
        message.push_front(0x00);
        message.push_front(EMPTY);
        break;
    case OBJECT_IDENTIFIER:
        std::string tempoid;
        uint32_t tempoidlen;
        cvrt::oidToRaw(value, tempoid, tempoidlen); //need funtion to check if it is a valid oid. maybe new class named oid.
        message.push_front(tempoid);
        message.push_front(cvrt::convertIntAccordingToBER(tempoidlen));
        message.push_front(OBJECT_IDENTIFIER);
        break;
    case SEQUENCE:
                                        //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    case IPADDRESS:
        IpAddress ip = new IpAddress(value);
        if(ip.isValidateIpAddress())
        {
            message.push_front(value);              //check welk format er gebruikt word.
            message.push_front(value.size());
            message.push_front(IPADDRESS);
            break;
        }
        else
        {
            abort();        //throw exception
        }
    case COUNTER32:
                                        //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    case GAUGE32:
                                        //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    case TIMETICKS:
        uint32_t tempvalue = std::stoi(value);
        message.push_front(tempvalue);
        if(tempvalue < 256)
            message.push_front(0x01);
        else if(tempvalue < 65536)
            message.push_front(0x02);
        else if(tempvalue < 16777216)
            message.push_front(0x03);
        else
            message.push_front(0x04);
        message.push_front(TIMETICKS);
        break;
    case OPAQUE:
                                        //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    case NASPADDRESS:
                                        //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    case COUNTER64:
                                        //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    case UINTEGER32:
                                        //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    default:
        break;
    }

    cvrt::oidToRaw(oid, constructedoid);
    message.push_front(constructedoid);
    message.push_front(cvrt::convertIntAccordingToBER(constructedoid.size()));
    message.push_front(0x06);
    verbindlistlen = message.size();
    message.push_front(cvrt::convertIntAccordingToBER(verbindlistlen));
    message.push_front(0x30);
    verbindfieldlen = message.size();
    message.push_front(cvrt::convertIntAccordingToBER(verbindfieldlen));
    message.push_front(0x30);
    message.push_front((char)0x00);
    message.push_front(0x01);
    message.push_front(0x02);
    message.push_front((char)0x00);
    message.push_front(0x01);
    message.push_front(0x02);
    message.push_front(cvrt::convertIntAccordingToBER(rand() % 255 + 1));
    message.push_front(0x01);
    message.push_front(0x02);
    pdulen = message.size();
    message.push_front(pdulen);
    message.push_front(this->messagetype);
    message.push_front(this->communityString);
    message.push_front(this->communityString.size());
    message.push_front(0x04);
    message.push_front((char)0x00);
    message.push_front(0x01);
    message.push_front(0x02);
    messagelen = message.size();
    message.push_front(cvrt::convertIntAccordingToBER(messagelen));
    message.push_front(0x30);
    return message;
}


