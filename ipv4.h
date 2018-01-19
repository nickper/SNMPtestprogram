#ifndef IPV4_H
#define IPV4_H

#include "IpAddress.h"

class IPv4 : public IpAddress
{
public:
    IPv4(std::string ipAddress);
    virtual ~IPv4();
    virtual bool isValidateIpAddress();
    virtual std::string getIpAddress();
    virtual void setIpAddress(std::string ipAddress);
private:
    int valid_digit(char *ip_str);
    std::string ipAddress;
};

#endif // IPV4_H
