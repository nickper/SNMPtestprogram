#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

enum {INTEGER=0x02, BITSTRING=0x03, OCTETSTRING=0x04, EMPTY=0x05, OBJECT_IDENTIFIER=0x06, SEQUENCE=0x30, IPADDRESS=0x40, COUNTER32=0x41, GAUGE32=0x42, TIMETICKS=0x43, OPAQUE=0x44, NASPADDRESS=0x45, COUNTER64=0x46, UINTEGER32=0x47} enumvalue;
enum {GET=0xa0, GETNEXT=0xa1, RESPONSE=0xa2, SET=0xa3, TRAP=0xa4, GETBULK=0xa5, INFORM=0xa6, TRAPv2=0xa7} enumtype;

class Message
{
public:
    virtual ~Message();
    virtual void sendMessage(std::string value, enumvalue valuetype) =0;
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
