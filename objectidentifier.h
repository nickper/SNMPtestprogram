#ifndef OBJECTIDENTIFIER_H
#define OBJECTIDENTIFIER_H

#include <string>

class ObjectIdentifier
{
public:
    ObjectIdentifier();
    ObjectIdentifier(std::string oid);
    std::string getObjectIdentifier() const;
    bool setObjectIdentifier(std::string oid);
private:
    bool isValidObjectIdentifier(std::string oid);
    int valid_digit(char *ip_str);
    std::string oid;
};

#endif // OBJECTIDENTIFIER_H
