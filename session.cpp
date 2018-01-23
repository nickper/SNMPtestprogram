#include "session.h"
#include "QThread"

Session::Session(const std::string &agentAddress, int16_t agentPort, int16_t socketPort):   QObject(), agentAddress(new QHostAddress(QString::fromStdString(agentAddress))), agentPort(agentPort), socketPort(socketPort)
{
    udpSocket.bind(socketPort);
}

Session::~Session()
{
    delete agentAddress;
}

void Session::sendMessage(std::deque<char> &sendingarray )
{
    QByteArray array;
    for(int i = 0; i < sendingarray.size(); i++)
    {
        array.push_back(sendingarray.at(i));
    }
    qint16 agentport = agentPort;
    udpSocket.writeDatagram(array, array.size(), *this->agentAddress, agentport);
}

std::deque<char> Session::receiveMessage()
{
    int timeoutTimer = 0;
    while(timeoutTimer <= 3000)
    {
        if(udpSocket.hasPendingDatagrams())
        {
            QByteArray receivedDatagram;
            std::deque<char> tempdeque;
            receivedDatagram.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(receivedDatagram.data(), receivedDatagram.size());
            for(int i = 0; i < receivedDatagram.size(); i++)
            {
                tempdeque.push_back(receivedDatagram.at(i));
            }
            return tempdeque;
        }
        QThread::msleep(1);
        timeoutTimer++;
    }
    throw -1;                                //should trow exception
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
    return agentAddress->toString().toStdString();
}

int16_t Session::getAgentPort() const
{
    return agentPort;
}

int16_t Session::getSocketPort() const
{
    return socketPort;
}
