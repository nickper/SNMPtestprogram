#ifndef SNMPV1_H
#define SNMPV1_H

#include <string>
#include "snmpmessage.h"

//class Snmpmessage;
class Snmpv1 : public Snmpmessage
{
public:
    Snmpv1(std::string ipAddress, uint8_t type, uint16_t sendingPort, uint16_t receivingPort);
    ~Snmpv1();
    virtual void sendMessage(std::string str);
    virtual void sendMessage(int32_t i, std::string str, const std::string valuetype);
    virtual void sendMessage(std::string str1, std::string str2);
    virtual std::string getError();
    virtual std::string getVersion();
    void setCommunityString(std::string communityString);

private:
    virtual std::string deciperErrorCode();
    void constructMessage(std::string str);
    void constructMessage(int32_t i, std::string str, const std::string valuetype);
    void constructMessage(std::string str1, std::string str2);

    virtual void storeValue();
    std::string communityString;
};

#endif // SNMPV1_H
