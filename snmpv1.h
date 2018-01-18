#ifndef SNMPV1_H
#define SNMPV1_H

#include <string>
#include "snmpmessage.h"
#include <deque>

//class Snmpmessage;
class Snmpv1 : public Snmpmessage
{
public:
    Snmpv1(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort);
    ~Snmpv1();
    virtual void sendMessage(std::string &value, enumvalue &valuetype);
    virtual std::string getError();
    virtual std::string getVersion();
    void setCommunityString(std::string communityString);

private:
    virtual std::string deciperErrorCode();
    std::deque constructMessage(std::string &value, enumvalue &valuetype);

    virtual void storeValue();
    std::string communityString;
};

#endif // SNMPV1_H
