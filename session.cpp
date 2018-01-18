#include "session.h"

Session::Session(const std::string &agentAddress, int16_t agentPort, int16_t socketPort):   QObject(), agentAddress(new QHostAddress(QString::fromStdString(agentAddress))), agentPort(agentPort), socketPort(socketPort)
{
    udpSocket.bind(socketPort);
}

Session::~Session()
{
    delete agentAddress;
}

void Session::sendMessage(std::deque &sendingarray )
{
    const QByteArray array;
    for(int i = 0; i < sendingarray.size(); i++)
    {
        array.push_back(sendingarray.at(i));
    }
    udpSocket.writeDatagram(array, array.size(), agentAddress, agentPort);
}

std::deque Session::receiveMessage()
{
    int timeoutTimer = 0;
    while(timeoutTimer <= 3000)
    {
        if(udpSocket.hasPendingDatagrams())
        {
            QByteArray receivedDatagram;
            std::deque tempque;
            receivedDatagram.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(receivedDatagram.data(), receivedDatagram.size());
            for(int i = 0; i < receivedDatagram.size(); i++)
            {
                tempque.push_back(receivedDatagram.at(i));
            }
            return;
        }
        QThread::msleep(1);
        timeoutTimer++;
    }
    return;     //should trow exception     //not necessary
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
