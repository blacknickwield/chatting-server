#ifndef FILESERVER_H
#define FILESERVER_H

#include <QObject>
#include <QTcpServer>
#include <tcpclient.h>
class FileServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit FileServer(QObject* parent = nullptr);

protected:
    virtual void incomingConnection(qintptr socketDescriptor);
};

#endif // FILESERVER_H
