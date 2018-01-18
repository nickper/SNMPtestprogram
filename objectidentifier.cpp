#include "objectidentifier.h"

ObjectIdentifier::ObjectIdentifier(std::string oid)
{
    if(isValidObjectIdentifier(oid))
        this->oid = oid;
    else
        abort();
}

bool ObjectIdentifier::isValidObjectIdentifier(std::string oid)
{
    int i, num;
    char *ptr;

    if (oid == NULL)
        return false;

    ptr = strtok(oid, '.');

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
            ptr = strtok(NULL, '.');
        } else
            return false;
    }
    return true;
}

std::string ObjectIdentifier::getObjectIdentifier() const
{
    return this->oid;
}

bool ObjectIdentifier::setObjectIdentifier(std::string oid)
{
    if(isValidObjectIdentifier(oid))
        this->oid = oid;
    else
        return false;
    return true;
}

int ObjectIdentifier::valid_digit(char *ip_str)
{
    while (*ip_str) {
        if (*ip_str >= '0' && *ip_str <= '9')
            ++ip_str;
        else
            return false;
    }
    return true;
}
