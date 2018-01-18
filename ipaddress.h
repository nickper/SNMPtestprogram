#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <string>

class IpAddress
{
public:
    IpAddress();
    virtual bool isValidateIpAddress() =0;
    virtual std::string getIpAddress() = 0;
    virtual void setIpAddress(std::string) = 0;
private:

};

#endif // IPADDRESS_H
