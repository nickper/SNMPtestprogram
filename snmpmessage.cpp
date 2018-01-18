#include "snmpmessage.h"
#include "session.h"

Snmpmessage::Snmpmessage(std::string ipAddress, uint8_t type, uint16_t /*sendingPort*/, uint16_t /*receivingPort*/): ipAddress(ipAddress), valuetype(enumvalue(type))/*, sendingPort(sendingPort), receivingPort(receivingPort)*/ //Sending and receiving port bestaan niet, aanmaken of verwijderen.
{
    createSession();
}

Snmpmessage::~Snmpmessage()
{
    deleteSession();
}

void Snmpmessage::createSession()
{
    this->session = new Session(this->ipAddress, sendingPort, receivingPort);// sendingport and receiving port bestaan niet
}

void Snmpmessage::deleteSession()
{
    delete this->session;
}

std::string Snmpmessage::getValue()
{
    return this->value;
}

//IpAddress Snmpmessage::getIpAddress()
//{
//    return IpAddress(this->ipaddress);
//}

enumtype Snmpmessage::getMessageType()
{
    return this->messagetype;
}

uint8_t Snmpmessage::getSendingPort()
{
    return this->sendingPort;//'Bestaat niet
}

uint8_t Snmpmessage::getReceivingPort()
{
    return this->receivingPort;// bestaat niet
}
