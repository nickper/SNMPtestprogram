#include "session.h"

#include "QThread"

Session::Session(const std::string &agentAddress, uint16_t agentPort, uint16_t socketPort):   QObject(), agentAddress(new QHostAddress(QString::fromStdString(agentAddress))), agentPort(agentPort), socketPort(socketPort)
{
    udpSocket.bind(socketPort);
}

Session::~Session()
{
    delete agentAddress;
}

void Session::sendMessage(char *sendingBytearray , char *receivingBytearray , const int16_t sendingsize , int16_t &receivinglenght)
{
    const QByteArray datagram = QByteArray(reinterpret_cast<char*>(sendingBytearray), sendingsize);
    uint8_t attemps = 0;
    int timeoutTimer = 0;

    while(attemps < 3)
    {
        // SEND IT !!!
        udpSocket.writeDatagram(datagram, datagram.size(), *agentAddress, agentPort);
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
                receivedDatagram.resize(static_cast<int>(udpSocket.pendingDatagramSize())); //TODO: check conversion
                udpSocket.readDatagram(receivedDatagram.data(), receivedDatagram.size());
                receivinglenght = receivedDatagram.size(); //TODO: size is int receivingbla is int16_t
                receivingBytearray = new char[receivinglenght];
                receivingBytearray = receivedDatagram.data();
                return;
            }
            QThread::msleep(1);
            timeoutTimer++;
        }

    }
    return;     //should trow exception
}

void Session::setAgentAddress(QHostAddress* agentAddress)
{
    this->agentAddress = agentAddress;
}

void Session::setAgentPort(int16_t agentPort)
{
    this->agentPort = agentPort;//TODO Conversion
}

void Session::setSocketPort(int16_t socketPort)
{
    this->socketPort = socketPort;// TODO: Conversion
    udpSocket.bind(this->socketPort);
}

QHostAddress* Session::getAgentAddress() const
{
    return this->agentAddress;
}

int16_t Session::getAgentPort() const
{
    return this->agentPort;// TODO: Conversion
}

int16_t Session::getSocketPort() const
{
    return this->socketPort;// TODO: Conversion
}
