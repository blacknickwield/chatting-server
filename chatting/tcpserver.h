#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <tcpclient.h>
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

signals:

protected:
    virtual void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
