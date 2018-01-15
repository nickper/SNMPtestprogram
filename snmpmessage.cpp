#include "snmpmessage.h"
#include "session.h"

Snmpmessage::Snmpmessage(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort): ipAddress(ipAddress), type(type), sendingPort(sendingPort), receivingPort(receivingPort)
{
    createSession();
}

Snmpmessage::~Snmpmessage()
{
    deleteSession();
    delete value;
    delete receivedDatagram[];
    delete senddedDatagram[];
    delete oid;
    delete ipAddress;
}

void Snmpmessage::createSession()
{
    session = new Session(ipAddress, sendingPort, receivingPort);
}

void Snmpmessage::deleteSession()
{
    delete Session;
}

std::string Snmpmessage::getValue()
{
    return value;
}

std::string Snmpmessage::getIpAddress()
{
    return ipAddress;
}

uint8_t Snmpmessage::getMessageType()
{
    return type;
}

uint8_t Snmpmessage::getSendingPort()
{
    return sendingPort;
}

uint8_t Snmpmessage::getReceivingPort()
{
    return receivingPort;
}
