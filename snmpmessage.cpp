#include "snmpmessage.h"
#include "session.h"

Snmpmessage::Snmpmessage(IpAddress ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort): ipAddress(ipAddress.getIpAddress()), type(type), sendingPort(sendingPort), receivingPort(receivingPort)
{
    createSession();
}

Snmpmessage::~Snmpmessage()
{
    deleteSession();
}

void Snmpmessage::createSession()
{
    this->session = new Session(ipAddress, sendingPort, receivingPort);
}

void Snmpmessage::deleteSession()
{
    delete this->session;
}

std::string Snmpmessage::getValue()
{
    return this->value;
}

IpAddress Snmpmessage::getIpAddress()
{
    return IpAddress(this->ipaddress);
}

enumtype Snmpmessage::getMessageType()
{
    return this->type;
}

uint8_t Snmpmessage::getSendingPort()
{
    return this->sendingPort;
}

uint8_t Snmpmessage::getReceivingPort()
{
    return this->receivingPort;
}
