#include "fileserver.h"
#include <QDebug>
#include "server.h"

FileServer::FileServer(QObject* parent) :
    QTcpServer(parent)
{

}

void FileServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "file come";
    TcpFile* client = new TcpFile(this);
    client->setSocketDescriptor(socketDescriptor);

    Server* server = Server::getInstance();

    foreach(auto clientSocket, server->getOnlineClients())
    {
        if(clientSocket->peerAddress() == client->peerAddress())
        {
            client->setUser(clientSocket->getUser());
            server->addFileClient(client);
            qDebug() << "88888";
            return ;
        }
    }
    qDebug() << "hhhhhhhh";
}
