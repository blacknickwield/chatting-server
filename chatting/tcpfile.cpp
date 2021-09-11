#include "tcpfile.h"
#include <QDebug>
#include "server.h"
TcpFile::TcpFile(QObject* parent) :
    QTcpSocket(parent), user(nullptr)
{
    connect(this, &TcpFile::readyRead,
            this, &TcpFile::sendFileSocketToServer);
    connect(this, &TcpFile::signalSendFileSocketToServer,
            Server::getInstance(), &Server::transmitFile);
}


User* TcpFile::getUser()
{
    return user;
}

void TcpFile::setUser(User *user)
{
    this->user = user;
}

void TcpFile::sendFileSocketToServer()
{
    emit signalSendFileSocketToServer(this);
}
