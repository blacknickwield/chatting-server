#ifndef TCPFILE_H
#define TCPFILE_H

#include <QObject>
#include <QTcpSocket>
#include "user.h"
#include <QDataStream>
#include <QFile>
class TcpFile : public QTcpSocket
{
    Q_OBJECT
public:
    TcpFile(QObject* parent = nullptr);

    User* getUser();
    void setUser(User*);

    // file test
    bool isStart = true;
    //QFile file;
    QString fileName;
    qint64 fileSize = 0;
    qint64 receiveSize = 0;

    int senderId;
    int receiverId;

    TcpFile* targetFileClient;
    // file test
signals:
    void signalSendFileSocketToServer(TcpFile*);

private slots:
    void sendFileSocketToServer();
private:
    User* user;
};

#endif // TCPFILE_H
