#ifndef SNMPMESSAGE_H
#define SNMPMESSAGE_H

//#include <string>
#include "message.h"

class Session;
class Snmpmessage : public Message
{
public:
    Snmpmessage(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort);
    ~Snmpmessage();
    virtual void sendMessage() =0;
    virtual std::String getError() = 0;
    virtual std::string getVersion() = 0;

    virtual void createSession();
    virtual void deleteSession();
    virtual std::string getValue();
    virtual std::string getIpAddress();
    virtual uint8_t getMessageType();
    virtual uint8_t getSendingPort();
    virtual uint8_t getReceivingPort();

private:
    virtual std::string deciperErrorCode() =0;
    virtual void storeValue()=0;

    std::string oid;
    std::string ipAddress;
    std::string value;
    uint8_t type;
    uint8_t error;
    uint8_t seqId;
    uint16_t sendingPort;
    uint16_t receivingPort;
    int16_t receivingsize;
    int16_t sendingsize;
    char receivedDatagram[];
    char senddedDatagram[];

    Session session;
};

#endif // SNMPMESSAGE_H

