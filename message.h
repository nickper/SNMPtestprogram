#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "ipaddress.h"

enum enumvalue {INTEGER=0x02, BITSTRING=0x03, OCTETSTRING=0x04, EMPTY=0x05, OBJECT_IDENTIFIER=0x06, SEQUENCE=0x30, IPADDRESS=0x40, COUNTER32=0x41, GAUGE32=0x42, TIMETICKS=0x43, OPAQUE=0x44, NASPADDRESS=0x45, COUNTER64=0x46, UINTEGER32=0x47, DEFAULT};
enum enumtype {GET=0xa0, GETNEXT=0xa1, RESPONSE=0xa2, SET=0xa3, TRAP=0xa4, GETBULK=0xa5, INFORM=0xa6, TRAPv2=0xa7};
enum enumerror {NOERROR=0x00, TOOBIG=0x01, NOSUCHNAME=0x02, BADVALUE=0x03, READONLY=0x04, GENERR=0x05,NOACCESS=0x06,WRONGTYPE=0x07,WRONGLENGHT=0x08,WRONGENCODING=0x09,WRONGVALUE=0x10,NOCREATION=0x11,INCOSISTENTVALUE=0x12,RESOURCEUNAVAILABLE=0x13,COMMITFAULED=0x14,UNDOFAILED=0x15,AUTHORIZATIONERROR=0x16,NOTWRITABLE=0x17,INCONSISTENDNAME=0x18};

class Message
{
public:
    virtual ~Message();
    virtual void sendMessage(std::string value, enumvalue valuetype) =0;
    virtual void createSession() =0;
    virtual void deleteSession() = 0;
    virtual std::string getValue() = 0;
    virtual enumerror getError() = 0;
    //virtual IpAddress getIpAddress() =0; //Mag IpAddress niet gebruiken als return, is abstracte klasse, zijn geen objecten van te maken.
    virtual std::string getIpAddress() = 0;
    virtual enumtype getMessageType() =0;
    virtual uint8_t getPort() = 0;

private:
    virtual std::string deciperErrorCode() = 0;
};

#endif // MESSAGE_H
