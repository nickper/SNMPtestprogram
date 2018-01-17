#include "session.h"

Session::Session(const std::string &agentAddress, int16_t agentPort, int16_t socketPort):   QObject(), agentAddress(new QHostAddress(QString::fromStdString(agentAddress))), agentPort(agentPort), socketPort(socketPort)
{
    udpSocket.bind(socketPort);
}

Session::~Session()
{
    delete agentAddress;
}

void Session::sendMessage(const char *sendingBytearray , char *receivingBytearray , const int16_t sendingsize , int16_t &receivinglenght)
{
    const QByteArray datagram = QByteArray(reinterpret_cast<char*>(sendingBytearray), sendingsize);
    uint8_t attemps = 0;
    int timeoutTimer = 0;

    while(attemps < 3)
    {
        // SEND IT !!!
        udpSocket.writeDatagram(datagram, datagram.size(), agentAddress, agentPort);
        if(attemps >= 2)
            timeoutTimer = 2500;
        else
            timeoutTimer = 0;

        // Receive it
        while(timeoutTimer <= 3000)
        {
            if(udpSocket.hasPendingDatagrams())
            {
                QByteArray receivedDatagram;
                receivedDatagram.resize(udpSocket.pendingDatagramSize());
                udpSocket.readDatagram(receivedDatagram.data(), receivedDatagram.size());
                receivinglenght = receivedDatagram.size();
                receivingBytearray = new char[receivinglenght];
                receivingBytearray = receivedDatagram.constData();
                return;
            }
            QThread::msleep(1);
            timeoutTimer++;
        }

    }
    return;     //should trow exception
}

void Session::setAgentAddress(const std::string agentAddress)
{
    this->agentAddress = agentAddress;
}

void Session::setAgentPort(int16_t agentPort)
{
    this->agentPort = agentPort;
}

void Session::setSocketPort(int16_t socketPort)
{
    this->socketPort = socketPort;
    udpSocket.bind(this->socketPort);
}

std::string Session::getAgentAddress() const
{
    return agentAddress;
}

int16_t Session::getAgentPort() const
{
    return agentPort;
}

int16_t Session::getSocketPort() const
{
    return socketPort;
}
