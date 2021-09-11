#include "tcpclient.h"
#include "server.h"
TcpClient::TcpClient(QObject* parent) :
    QTcpSocket(parent), user(nullptr)
{
    connect(this, &TcpClient::readyRead,
            this, &TcpClient::sendClientSocketToServer);
    connect(this, &TcpClient::signalSendClientSocketToServer,
            Server::getInstance(), &Server::receiveMessage);
}


void TcpClient::sendClientSocketToServer()
{
    emit signalSendClientSocketToServer(this);
}


User* TcpClient::getUser()
{
    return user;
}

void TcpClient::setUser(User* user)
{
    this->user = user;
}

void TcpClient::sendMessage(QByteArray& message)
{
    write(message);
    // warning: new!!
    flush();
}
