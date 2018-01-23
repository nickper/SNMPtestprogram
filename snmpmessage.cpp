#include "snmpmessage.h"
#include "session.h"

Snmpmessage::Snmpmessage(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort): ipAddress(ipAddress), valuetype(enumvalue(type)), oid(ObjectIdentifier())
{
    createSession(sendingPort, receivingPort);
    //this->oid = ObjectIdentifier("1.3");
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

ObjectIdentifier Snmpmessage::getOid()
{
    return this->oid;
}

void Snmpmessage::setOid(std::string oid)
{
    this->oid = ObjectIdentifier(oid);
}
