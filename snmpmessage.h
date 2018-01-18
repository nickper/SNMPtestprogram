#ifndef SNMPMESSAGE_H
#define SNMPMESSAGE_H

#include "message.h"
#include "session.h"
#include "ipaddress.h"
#include "objectidentifier.h"


class Session;
class Snmpmessage : public Message
{
public:
    Snmpmessage(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort);
    ~Snmpmessage();
    virtual void ssendMessage(std::string value, enumvalue valuetype) =0;
    virtual enumerror getError() = 0;
    virtual std::string getVersion() = 0;

    virtual void createSession();
    virtual void deleteSession();
    virtual std::string getValue();
    virtual IpAddress getIpAddress();
    virtual enumtype getMessageType();
    virtual uint8_t getSendingPort();
    virtual uint8_t getReceivingPort();

protected:
    virtual std::string deciperErrorCode() =0;
    virtual void storeValue()=0;

    ObjectIdentifier oid;
    IpAddress ipaddress;
    std::string ipAddress;
    std::string valuetype;
    std::string value;
    enumvalue valuetype;
    enumtype messagetype;
    enumerror error;
    uint8_t seqId;

    Session *session;
};

#endif // SNMPMESSAGE_H

