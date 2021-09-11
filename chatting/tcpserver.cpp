#include "tcpserver.h"
#include "tcpclient.h"
#include "server.h"
#include <QDebug>
#include "mainwidget.h"
extern int number;
TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    TcpClient* client = new TcpClient(this);
    client->setSocketDescriptor(socketDescriptor);
    Server* server = Server::getInstance();
    server->addLoginClient(client);
}
