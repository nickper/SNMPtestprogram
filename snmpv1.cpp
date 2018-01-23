#include "snmpv1.h"
#include "snmpmessage.h"
#include "cvrt.h"
#include "session.h"
#include "ipv4.h"

Snmpv1::Snmpv1(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort) : Snmpmessage(ipAddress, type, sendingPort, receivingPort)
{
    createSession(sendingPort, receivingPort);
}

Snmpv1::~Snmpv1()
{
    deleteSession();
}

void Snmpv1::sendMessage(std::string &value, enumvalue &valuetype)
{
    std::deque<char> tempDeque = this->constructMessage(value, valuetype);
    this->session->sendMessage(tempDeque);
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
void Snmpv1::getValue(std::deque<char> receivedMessage)
{
    std::string communityStringreceived = "";
    std::string oid = "1.3";

    uint8_t messageType =0;
    uint8_t seqID =0;
    int16_t oidLenght = 0;
    uint8_t version = 255;
    uint8_t errorCode = 0;
    int16_t receivedoidlen = 0;
    uint64_t verbindFieldLenght = 0;
    uint64_t verbindListLenght = 0;
    uint64_t valueLength = 0;
    uint64_t longlenght = 0;
    uint64_t pduLenght = 0;
    uint64_t messageIndex = 1;
    uint8_t lenght = receivedMessage.at(1);

    uint j, i;

    // ---------- retreive total message length ------------
    if(lenght >= 127)
    {
        j = 0;
        for(int i = lenght-128 -1; i >= 0 ; i--)
        {
            longlenght = (receivedMessage.at(i+2) << ((j)*8)) | longlenght;
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
    version = receivedMessage.at(messageIndex);
    messageIndex += 2;

    // ---------- retreive community string ------------
    for(i = 1; i <= (unsigned)receivedMessage.at(messageIndex); i++)
    {
        communityStringreceived += receivedMessage.at(messageIndex+i);
    }
    messageIndex += i;

    // ---------- retreive SNMP message type ------------
    messageType = receivedMessage.at(messageIndex);
    messageIndex += 1;

    // ---------- retreive total PDU length ------------
    if((unsigned)receivedMessage.at(messageIndex) >= 127)
    {
        j = 0;
        for(uint8_t i = (unsigned)receivedMessage.at(messageIndex)-128; i > 0 ; i--)
        {
            pduLenght = (receivedMessage.at(messageIndex+i)) << ((j)*8) | pduLenght;
            j++;
        }
        messageIndex += j;
    }
    else
    {
        pduLenght = receivedMessage.at(messageIndex);
    }
    messageIndex +=3;

    // ---------- retreive sequence ID ------------
    seqID = receivedMessage.at(messageIndex);
    messageIndex +=3;

    // ---------- return error code if its occurs ------------
    if(receivedMessage.at(messageIndex) != 0)
    {
        errorCode = receivedMessage.at(messageIndex);
    }
    messageIndex +=5;

    // ---------- retreive total verbindfield length ------------
    if((unsigned)receivedMessage.at(messageIndex) >= 127)
    {
        j = 0;
        for(uint8_t i = (unsigned)receivedMessage.at(messageIndex)-128; i > 0 ; i--)
        {
            verbindFieldLenght = (receivedMessage.at(messageIndex+i)) << ((j)*8) | verbindFieldLenght;
            j++;
        }
        messageIndex += j;
    }
    else
    {
        verbindFieldLenght = receivedMessage.at(messageIndex);
    }
    messageIndex +=2;

    // ---------- retreive total verbindlist length ------------
    if((unsigned)receivedMessage.at(messageIndex) >= 127)
    {
        j = 0;
        for(uint8_t i = (unsigned)receivedMessage.at(messageIndex)-128; i > 0 ; i--)
        {
            verbindListLenght = (receivedMessage.at(messageIndex+i)) << ((j)*8) | verbindListLenght;
            j++;
        }
        messageIndex += j;
    }
    else
    {
        verbindListLenght = receivedMessage.at(messageIndex);
    }
    messageIndex +=2;

    // ---------- retreive OID  ------------
    j = 1;
    oidLenght = receivedMessage.at(messageIndex);
    char tempoidchar[oidLenght];
    for(int i = 1; i < oidLenght; i++)
    {
        tempoidchar[i-1] = receivedMessage.at(messageIndex+i);
    }

    cvrt::rawToOid(tempoidchar, oid, oidLenght, receivedoidlen);
    messageIndex += oidLenght + 1;

    // ---------- retreive value ------------
    messageIndex +=2;
    switch ((unsigned)receivedMessage.at(messageIndex-1)) {
    case INTEGER:
    {
        valuetype = INTEGER;
        uint tempvalue = 0;
        valueLength = receivedMessage.at(messageIndex);
        //messageIndex +=1;
        j=0;
        for(int i = valueLength; i > 0 ; i--)
        {
            value = (receivedMessage.at(messageIndex+i)) << ((j)*8) | tempvalue;
            j++;
        }
        value = tempvalue;
        messageIndex += j;
        break;
    }
    case BITSTRING:
    {
        valuetype = BITSTRING;
        value = "BITSTRING";
        break;
    }
    case OCTETSTRING:
    {
        valuetype = OCTETSTRING;
        std::string tempvalue = "";
        if((uint8_t)receivedMessage.at(messageIndex) >= 127)
        {
            j = 0;
            for(uint8_t i = ((uint8_t)receivedMessage.at(messageIndex))-128; i > 0  ; i--)
            {
                valueLength = (uint8_t)(receivedMessage.at(messageIndex+i)) << ((j)*8) | valueLength;
                j++;
            }
        }
        else
        {
            valueLength = receivedMessage.at(messageIndex);
        }
        messageIndex +=1;
        for(i = 1; i <= valueLength; i++)
        {
            tempvalue += receivedMessage.at(messageIndex);
            messageIndex +=1;
        }
        value = tempvalue;
        break;
    }
    case EMPTY:
    {
        valuetype = EMPTY;
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
    case OBJECT_IDENTIFIER:
    {
        valuetype = OBJECT_IDENTIFIER;
        value = "OID";
        break;
    }
    case SEQUENCE:
    {

        valuetype = SEQUENCE;
        value = "SEQENCE";
        break;
    }
    case IPADDRESS:
    {
        valuetype = IPADDRESS;
        value = "IP address";
        break;
    }
    case COUNTER32:
    {
        valuetype =COUNTER32;
        value = "32 Bits Counter";
        break;
    }
    case GAUGE32:
    {
        valuetype = GAUGE32;
        value = "32 Bits Gauge";
        break;
    }
    case TIMETICKS:
    {
        valuetype = TIMETICKS;
        uint32_t tempvalue = 0;
        valueLength = receivedMessage.at(messageIndex);
        //messageIndex +=1;
        j=0;
        for(int i = valueLength; i > 0 ; i--)
        {
            tempvalue =  ((unsigned)receivedMessage.at(messageIndex+i)) << ((j)*8) | tempvalue;
            j++;
        }
        value = std::to_string(tempvalue);
        break;
    }
    case OPAQUE:
    {
        valuetype = OPAQUE;
        value = "Opaque";
        break;
    }
    case NASPADDRESS:
    {
        valuetype = NASPADDRESS;
        value = "NsapAdress";
        break;
    }
    case COUNTER64:
    {
        valuetype = COUNTER64;
        value = "64 Bits Counter";
        break;
    }
    case UINTEGER32:
    {
        valuetype = UINTEGER32;
        value = "Unsigned Int 32";
        break;
    }
    default:
        valuetype = DEFAULT;
        value = "Default";
        break;
    }
}

std::deque<char> Snmpv1::constructMessage(std::string &value, enumvalue &valuetype)
{
    std::deque<char> message;
    uint16_t verbindfieldlen = 0;
    uint16_t verbindlistlen = 0;
    uint16_t pdulen = 0;
    uint16_t messagelen = 0;

    std::string constructedoid = "";\

    //-------------construct value field---------------
    switch (valuetype) {
    case INTEGER:
    {
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
    }
    case BITSTRING:
    {                       //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    }
    case OCTETSTRING:
    {
        for(int i = 0; i < value.size(); i++)
        {
            message.push_front(value.at(i));
        }
        //message.push_front(value);
        std::string tempstring = cvrt::convertIntAccordingToBER(value.size());
                for(int i = 0; i < tempstring.size(); i++)
                {
                    message.push_front(tempstring.at(i));
                }
        message.push_front(OCTETSTRING);
        break;
    }
    case EMPTY:
    {
        message.push_front(0x00);
        message.push_front(EMPTY);
        break;
    }
    case OBJECT_IDENTIFIER:
    {
        ObjectIdentifier tempOid(value);
        std::string convertedOid = "";
        //uint32_t tempoidlen;
        cvrt::oidToRaw(tempOid.getObjectIdentifier(), convertedOid); //need funtion to check if it is a valid oid. maybe new class named oid.
        //message.push_front(convertedOid);
        //message.push_front(cvrt::convertIntAccordingToBER(convertedOid.size()));
        for(int i = 0; i < convertedOid.size(); i++)
        {
            message.push_front(convertedOid.at(i));
        }
        std::string tempstring = cvrt::convertIntAccordingToBER(value.size());
        for(int i = 0; i < tempstring.size(); i++)
        {
            message.push_front(tempstring.at(i));
        }

        message.push_front(OBJECT_IDENTIFIER);
        break;
    }
    case SEQUENCE:
    {                      //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    }
    case IPADDRESS:
    {
        IPv4 ip(value);
        if(ip.isValidateIpAddress())
        {
            for(int i = 0; i < value.size(); i++)
            {
                message.push_front(value.at(i));
            }
            //message.push_front(value);              //check welk format er gebruikt word.
            message.push_front(value.size());
            message.push_front(IPADDRESS);
            break;
        }
        else
        {
            abort();        //throw exception
        }
    }
    case COUNTER32:
    {                        //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    }
    case GAUGE32:
    {                                   //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    }
    case TIMETICKS:
    {
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
    }
    case OPAQUE:
    {                                   //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    }
    case NASPADDRESS:
    {                                   //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    }
    case COUNTER64:
    {                                    //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    }
    case UINTEGER32:
    {                                    //nog eens goed uitzoeken hoe deze encoding werkt
        break;
    }
    default:
    {
        break;
    }
    }

    //--------------construct the rest of the message------------
    std::string tempstring;
    cvrt::oidToRaw(oid.getObjectIdentifier(), constructedoid);
    //message.push_front(constructedoid);
    for(int i = 0; i < constructedoid.size(); i++)
    {
        message.push_front(constructedoid.at(i));
    }
    //message.push_front(cvrt::convertIntAccordingToBER(constructedoid.size()));
    tempstring = cvrt::convertIntAccordingToBER(constructedoid.size());
            for(int i = 0; i < tempstring.size(); i++)
    {
        message.push_front(tempstring.at(i));
    }
    message.push_front(0x06);
    verbindlistlen = message.size();
    //message.push_front(cvrt::convertIntAccordingToBER(verbindlistlen));
    tempstring = cvrt::convertIntAccordingToBER(verbindlistlen);
            for(int i = 0; i < tempstring.size(); i++)
    {
        message.push_front(tempstring.at(i));
    }
    message.push_front(0x30);
    verbindfieldlen = message.size();
    //message.push_front(cvrt::convertIntAccordingToBER(verbindfieldlen));
    tempstring = cvrt::convertIntAccordingToBER(verbindfieldlen);
            for(int i = 0; i < tempstring.size(); i++)
    {
        message.push_front(tempstring.at(i));
    }
    message.push_front(0x30);
    message.push_front((char)0x00);
    message.push_front(0x01);
    message.push_front(0x02);
    message.push_front((char)0x00);
    message.push_front(0x01);
    message.push_front(0x02);
    //message.push_front(cvrt::convertIntAccordingToBER(rand() % 255 + 1));
    tempstring = cvrt::convertIntAccordingToBER(rand() % 255 + 1);
            for(int i = 0; i < tempstring.size(); i++)
    {
        message.push_front(tempstring.at(i));
    }
    message.push_front(0x01);
    message.push_front(0x02);
    pdulen = message.size();
    message.push_front(pdulen);
    message.push_front(this->messagetype);
    //message.push_front(this->communityString);
            for(int i = 0; i < this->communityString.size(); i++)
    {
        message.push_front(this->communityString.at(i));
    }
    message.push_front(this->communityString.size());
    message.push_front(0x04);
    message.push_front((char)0x00);
    message.push_front(0x01);
    message.push_front(0x02);
    messagelen = message.size();
    //message.push_front(cvrt::convertIntAccordingToBER(messagelen));
    tempstring = cvrt::convertIntAccordingToBER(messagelen);
            for(int i = 0; i < tempstring.size(); i++)
    {
        message.push_front(tempstring.at(i));
    }
    message.push_front(0x30);
    return message;
}


