#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message
{
public:
    virtual void sendMessage() =0;
    virtual void createSession() =0;
    virtual void deleteSession() = 0;
    virtual std::string getValue() = 0;
    virtual std::string getError() = 0;
    virtual std::string getIpAddress() =0;
    virtual uint8_t getMessageType() =0;
    virtual uint8_t getPort() = 0;

private:
    virtual std::string deciperErrorCode() = 0;
};

#endif // MESSAGE_H
