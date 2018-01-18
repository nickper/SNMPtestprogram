#ifndef SESSION_H
#define SESSION_H

#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include <string>
#include <deque>

class Session : public QObject {

    Q_OBJECT

public:
    Session(const std::string &agentAddress, int16_t agentPort, int16_t socketPort);
    ~Session();
    void sendMessage(std::deque &sendingarray);
    std::deque receiveMessage();
    void setAgentAddress(const std::string agentAddress);
    void setAgentPort(int16_t agentPort);
    void setSocketPort(int16_t socketPort);

    std::string getAgentAddress() const;
    int16_t getAgentPort() const;
    int16_t getSocketPort() const;
private:
    QUdpSocket udpSocket;
    QHostAddress *agentAddress;
    int16_t agentPort;
    int16_t socketPort;
};

#endif // SESSION_H
