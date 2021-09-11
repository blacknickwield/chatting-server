#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include "user.h"
class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject* parent = nullptr);
    //void sendMessage(QString&);
    void sendMessage(QByteArray&);
    User* getUser();
    void setUser(User*);
private:
    User* user;
signals:
    void signalSendClientSocketToServer(TcpClient*);
private slots:
    void sendClientSocketToServer();
};

#endif // TCPCLIENT_H
