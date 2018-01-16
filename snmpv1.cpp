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

void Snmpv1::sendMessage(const std::string str)
{ 
    this->constructMessage(str);
    //this->session->sendMessage(sendedDatagram, receivedDatagram, sendingsize, receivingsize);
}


void Snmpv1::sendMessage(const int32_t i, const std::string str, const std::string valuetype)
{
    this->constructMessage(i, str, valuetype);
    //this->session->sendMessage(sendedDatagram, receivedDatagram, sendingsize, receivingsize);
}

void Snmpv1::sendMessage(const std::string str1, const std::string str2)
{
    this->constructMessage(str1, str2);
    //this->session->sendMessage(sendedDatagram, receivedDatagram, sendingsize, receivingsize);
}

std::string Snmpv1::getError()
{
    switch (this->error) {
        case 0:     std::cout << "no error" << std::endl;       return "no error";
        case 1:     std::cout << "error: TooBig" << std::endl;  return "TooBig"; //TODO ETC
        case 2:
            std::cout << "error: NoSuchName" << std::endl;
            return "NoSuchName";
            break;
        case 3:
            std::cout << "error: Badvalue" << std::endl;
            return "Badvalue";
            break;
        case 4:
            std::cout << "error: ReadOnly" << std::endl;
            return "ReadOnly";
            break;
        case 5:
            std::cout << "error: GenErr" << std::endl;
            return "GenErr";
            break;
        case 6:
            std::cout << "error: NoAccess" << std::endl;
            return "NoAccess";
            break;
        case 7:
            std::cout << "error: WrongType" << std::endl;
            return "WrongType";
            break;
        case 8:
            std::cout << "error: WrongLength" << std::endl;
            return "WrongLength";
            break;
        case 9:
            std::cout << "error: WrongEncoding" << std::endl;
            return "WrongEncoding";
            break;
        case 10:
            std::cout << "error: WrongValue" << std::endl;
            return "WrongValue";
            break;
        case 11:
            std::cout << "error: NoCreation" << std::endl;
            return "NoCreation";
            break;
        case 12:
            std::cout << "error: InconsistentValue" << std::endl;
            return  "InconsistentValue";
            break;
        case 13:
            std::cout << "error: ResourceUnavailable" << std::endl;
            return "ResourceUnavailable";
            break;
        case 14:
            std::cout << "error: CommitFauled" << std::endl;
            return "CommitFauled";
            break;
        case 15:
            std::cout << "error: UndoFailed" << std::endl;
            return "UndoFailed";
            break;
        case 16:
            std::cout << "error: AuthorizationError" << std::endl;
            return "AuthorizationError";
            break;
        case 17:
            std::cout << "error: NotWritable" << std::endl;
            return "NotWritable";
            break;
        case 18:
            std::cout << "error: InconsistentName" << std::endl;
            return "InconsistentName";
            break;
        default:
            std::cout << "unknown error occur" << std::endl;
            return "unknown";
            break;
        }
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
    //return something?
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

        char receivedDatagram[1];//TODO!!
        //uint8_t lenght = receivedDatagram;
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

void Snmpv1::constructMessage(const std::string str)
{
    uint16_t verbindfieldlen = 0;
    uint16_t verbindlistlen = 0;
    uint16_t pdulen = 0;
    uint16_t communityStringlen = str.length();
    int16_t constructedoidlen = 0;
    uint16_t totalmessagelen = 0;

    std::string constructedoid = "";\
    //sendedDatagram = "";
    std::string pdufield = "";

    cvrt::oidToRaw(oid, constructedoid, oid.length(), constructedoidlen);

    std::vector<char> sendedDatagram;//?

    sendedDatagram.push_back(0x06);
    sendedDatagram.push_back(cvrt::convertIntAccordingToBER(constructedoidlen));// ETC + constructedoid  + 0x05 + 0x00);
    verbindlistlen = strlen(sendedDatagram);
    sendedDatagram = 0x30 + cvrt::convertIntAccordingToBER(verbindlistlen) + sendedDatagram;
    verbindfieldlen = strlen(sendedDatagram);
    sendedDatagram = 0x02 + 0x01 + cvrt::convertIntAccordingToBER(rand() % 255 + 1) + 0x02 + 0x01+ (char)0x00 + 0x02 + 0x01 + (char)0x00 + 0x30 + cvrt::convertIntAccordingToBER(verbindfieldlen) + sendedDatagram;
    pdulen = strlen(sendedDatagram);
    pdufield = cvrt::constructPduField(type);
    sendedDatagram = 0x02 + 0x01 + (char)0x00 + 0x04 + cvrt::convertIntAccordingToBER(communityStringlen) + str + pdufield + cvrt::convertIntAccordingToBER(pdulen) + sendedDatagram;
    totalmessagelen = strlen(sendedDatagram);
    sendedDatagram = 0x30 + cvrt::convertIntAccordingToBER(totalmessagelen) + sendedDatagram;
    sendingsize = strlen(sendedDatagram);
}

void Snmpv1::constructMessage(const int32_t i, const std::string str, const std::string valuetype)
{

}

void Snmpv1::constructMessage(const std::string str1, const std::string str2)
{

}
