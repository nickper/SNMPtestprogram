#ifndef SESSION_H
#define SESSION_H

#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include <string>

class Session : public QObject {

    Q_OBJECT

public:
    Session(const std::string &agentAddress, uint16_t agentPort, uint16_t socketPort);
    ~Session();
    void sendMessage(char *sendingBytearray, char *receivingBytearray, const int16_t sendingsize, int16_t &receivinglenght);
    void setAgentAddress(QHostAddress* agentAddress);
    void setAgentPort(int16_t agentPort);
    void setSocketPort(int16_t socketPort);

    QHostAddress* getAgentAddress() const;
    int16_t getAgentPort() const;
    int16_t getSocketPort() const;
private:
    QUdpSocket udpSocket;
    QHostAddress *agentAddress;
    uint16_t agentPort;
    uint16_t socketPort;
};

#endif // SESSION_H
