#ifndef SNMPV1_H
#define SNMPV1_H

#include <string>
#include "snmpmessage.h"
#include <deque>

class Snmpv1 : public Snmpmessage
{
public:
    Snmpv1(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort);
    ~Snmpv1();
    virtual void sendMessage(std::string &value, enumvalue &valuetype);
    virtual enumerror getError();
    virtual std::string getVersion();
    void setCommunityString(std::string communityString);

private:
    virtual std::string deciperErrorCode();
    std::deque<char> constructMessage(std::string &value, enumvalue &valuetype);

    virtual void getValue(std::deque<char> receivedMessage);
    std::string communityString;
};

#endif // SNMPV1_H
