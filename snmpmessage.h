#ifndef SNMPMESSAGE_H
#define SNMPMESSAGE_H

#include "message.h"
#include "session.h"

class Session;
class Snmpmessage : public Message
{
public:
    Snmpmessage(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort);
    ~Snmpmessage();
    virtual void sendMessage(std::string str) = 0;
    virtual void sendMessage(int32_t i, std::string str, const std::string valuetype) = 0;
    virtual void sendMessage(std::string str1, std::string str2) = 0;
    virtual std::string getError() = 0;
    virtual std::string getVersion() = 0;

    virtual void createSession();
    virtual void deleteSession();
    virtual std::string getValue();
    virtual std::string getIpAddress();
    virtual uint8_t getMessageType();
    virtual uint8_t getSendingPort();
    virtual uint8_t getReceivingPort();

protected:
    virtual std::string deciperErrorCode() =0;
    virtual void storeValue()=0;

    std::string oid;
    std::string ipAddress;
    std::string valuetype;
    std::string value;
    uint8_t type;
    uint8_t error;
    uint8_t seqId;
    uint16_t sendingPort;
    uint16_t receivingPort;
    int16_t receivingsize;
    int16_t sendingsize;

    Session *session;
};

#endif // SNMPMESSAGE_H

