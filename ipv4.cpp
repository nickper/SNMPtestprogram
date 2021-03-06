#include "ipv4.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

IPv4::IPv4(std::string ipAddress)
{
    if(isValidateIpAddress())
        this->ipAddress = ipAddress;
    else
        abort();        //throw exception.
}

bool IPv4::isValidateIpAddress()
{
    int i, num, dots = 0;
    char *ptr;

    if (ipAddress == "")
        return false;

    char *cstr = &ipAddress[0u];

    ptr = std::strtok(cstr, '.');

    if (ptr == NULL)
        return false;

    while (ptr)
    {
        if (!valid_digit(ptr))
            return false;

        num = atoi(ptr);

        /* check for valid IP */
        if (num >= 0 && num <= 255)
        {
            /* parse remaining string */
            ptr = std::strtok(NULL, '.');
            if (ptr != NULL)
                ++dots;
        } else
            return false;
    }

    if (dots != 3)
        return false;
    return true;
}

int IPv4::valid_digit(char *ip_str)
{
    while (*ip_str)
    {
        if (*ip_str >= '0' && *ip_str <= '9')
            ++ip_str;
        else
            return false;
    }
    return true;
}

virtual std::string IPv4::getIpAddress()
{
    return this->ipAddress;
}

virtual void IPv4::setIpAddress(std::string ipAddress)
{
    if(isValidateIpAddress())
    {
        this->ipAddress = ipAddress
    }
    else
    {
        abort();        //throw exception
    }
}
