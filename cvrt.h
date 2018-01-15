#include <string>
#include <vector>
#include <iostream>

namespace cvrt
{

std::string int_to_hex(uint8_t i)
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(T)*2) << std::hex << i;
  return stream.str();
}

void hexstring2ascii(const std::string &in, std::string &out)
{
    out.clear();
    out.reserve(in.length() / 2);
    for (string::const_iterator p = in.begin(); p != in.end(); p++)
    {
       unsigned char c = hexval(*p);
       p++;
       if (p == in.end()) break; // incomplete last digit - should report error
       c = (c << 4) + hexval(*p); // + takes precedence over <<
       out.push_back(c);
    }
}

std::string rawToOid(const char* input[] , std::string &output , const int16_t inputlen , int16_t &outputlen)
{
    output = "1.3";
    for(int i=1; i <= inputlen ; i++)
    {
        output += ".";
        if((unsigned)input[i] <= 127)
        {
            output += std::to_string(input[i]);
        }
        else
        {
            uint8_t index = 0;
            uint32_t longoid = 0;
            uint8_t temp = input[i];
            temp &= ~128;
            longoid = longoid << ( index * 7) | temp;
            while ((uint8_t)input[i+index] > 127)
            {
                index++;
                temp = input[i+index];
                temp &= ~128;
                longoid = longoid << 7 | temp;
            }
            output += std::to_string(longoid);
            i += index;
        }
    }
}

void oidToRaw(const std::string input , char &output[] , const int16_t inputlen , int16_t &outputlen)
{
    std::vector<uint16_t> oidvector;

    int oidwalk = 0;
    std::string ;
    for(int i = 1; i < input.size(); i++)
    {
        oidwalk = 0;
        tempstring = "";
        while(input.at(i + oidwalk) != '.' && (i + oidwalk) < input.size())
        {
            tempstring += input.at(oidwalk);
            i++;
        }
        oidvector.push_back(std::stoi(tempstring));
    }

    std::string raw_str =R"(+)";                    //stores de '+' character in raw format. represents the 1.3. aan het begin van de oid
    for(int i = 1; i < oidvector.size(); i++)
    {
        raw_str += convertIntAccordingToBER(oidvector.at(i));
    }
    output = raw_str.size();
    output = raw_str;
}

std::string convertIntAccordingToBER(uint32_t const &valueToConvert)
{
    std::string temp =R"()";
    if(valueToConvert >= 127)
    {
        hexstring2ascii(int_to_hex(valueToConvert), temp);
    }
    else
    {
        std::string temp2 =R"()";
        if(valueToConvert <= 16383)
        {
            uint8_t xlow = valueToConvert & 0xff;
            uint8_t xhigh = (valueToConvert >> 8) + 128;
            hexstring2ascii(int_to_hex(xhigh), temp);
            hexstring2ascii(int_to_hex(xlow), temp2);
            temp += temp2;
        }
        else
        {
            if(valueToConvert > 2097151)
            {
                std::cout << "value to high" << std::endl;
                throw;
            }
            else
            {
                std::string temp3 =R"()";
            uint8_t xlow = valueToConvert & 0xff;
            uint8_t xmid = ((valueToConvert >> 8) & 0xff);
            uint8_t xhigh= (valueToConvert >> 16);
            if(xmid > 127)
            {
                xhigh *=2;
                xmid &= ~128;
            }
            if(xlow > 127)
            {
                xmid *=2;
                xlow &= ~128;;
                if(xmid > 127)
                {
                    xhigh *=2;
                    xmid &= ~128;
                }
            }
            hexstring2ascii(int_to_hex(xhigh), temp + 128);
            hexstring2ascii(int_to_hex(xmid), temp2 + 128);
            hexstring2ascii(int_to_hex(xlow), temp3);
            temp += temp2 + temp3;
            }
        }
    }
    return temp;
}

}

