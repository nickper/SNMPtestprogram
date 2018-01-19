#include "snmpmessage.h"
#include "session.h"

Snmpmessage::Snmpmessage(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort): ipAddress(ipAddress), valuetype(enumvalue(type))
{
    createSession(sendingPort, receivingPort);
}

Snmpmessage::~Snmpmessage()
{
    deleteSession();
}

void Snmpmessage::createSession(uint16_t sendingPort, uint16_t receivingPort)
{
    this->session = new Session(this->ipAddress, sendingPort, receivingPort);
}

void Snmpmessage::deleteSession()
{
    delete this->session;
}

std::string Snmpmessage::getValue()
{
    return this->value;
}

std::string Snmpmessage::getIpAddress()
{
    return this->ipAddress;
}

enumtype Snmpmessage::getMessageType()
{
    return this->messagetype;
}

uint8_t Snmpmessage::getSendingPort()
{
    return this->session->getAgentPort();
}

uint8_t Snmpmessage::getReceivingPort()
{
    return this->session->getSocketPort();
}
