#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "tcpserver.h"
#include "tcpclient.h"
#include "tcpfile.h"
#include "fileserver.h"
#include <QMap>


class Server : public QObject
{
    Q_OBJECT
public:
    static Server* getInstance();
    static Server* getInstance(int);
    void start(int messagePort, int filePort);
    void stop();
    QList<TcpClient*> getOnlineClients();
    QList<TcpClient*> getLoginClients();

    QList<TcpFile*> getFileClients();

    void addFileClient(TcpFile* fileClient);

    void addOnlineClient(TcpClient* onlineClient);
    void addLoginClient(TcpClient* loginClient);

    // 登陆
    void login(QJsonObject* object, TcpClient* client);

    // 注册
    void userRegister(QJsonObject* object, TcpClient* client);

    // 发送消息
    void sendMessage(QJsonObject* object, TcpClient* client);

    // 添加好友
    void addFriend(QJsonObject* object, TcpClient* client);

    // 添加好友返回信息
    void addNewFirendReply(QJsonObject* object, TcpClient* client);

    // 聊天记录
    void getChatLog(QJsonObject* object, TcpClient* client);

    // 退出登录
    void logout(QJsonObject* object, TcpClient* client);

    void addFriendReplyInfo(QJsonObject* object, TcpClient* client);

    //删除好友关系
    void deleteFriend(QJsonObject* object, TcpClient* client);

    //TODO 群聊
    void sendGroupMessage(QJsonObject* object, TcpClient* client);
private:
    explicit Server(QObject *parent = nullptr);
    // 单例对象
    static Server* singleton;

    // 消息传输服务器, 产生负责传输消息的套接字，监听6666端口
    TcpServer* tcpserver;

    // 文件传输服务器，产生传输文件的套接字，监听9999端口
    FileServer* fileServer;

    // 已登录的客户端套接字
    QMap<int, TcpClient*> onlineClients;

    // 传输文件的套接字
    QMap<int, TcpFile*> fileClients;

    // 正在登录中的客户端套接字
    QList<TcpClient*> loginClients;

signals:

public slots:
    void receiveMessage(TcpClient*);
    void transmitFile(TcpFile*);
};

#endif // SERVER_H
