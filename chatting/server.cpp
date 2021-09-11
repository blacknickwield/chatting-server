#include "server.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include "mainwidget.h"
#include "dao.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "enums.h"
#include <QJsonArray>
#include <QThread>
extern MainWidget* mw;
Server* Server::singleton = nullptr;

Server::Server(QObject *parent) :
    QObject(parent),
    tcpserver(new TcpServer),
    fileServer(new FileServer)
{

}

Server* Server::getInstance()
{


    if (singleton == nullptr)
    {
        singleton = new Server();
    }
    return singleton;
}

Server* Server::getInstance(int)
{
    return singleton;
}

void Server::start(int messagePort, int filePort)
{
    if (tcpserver->listen(QHostAddress::Any, messagePort) == false)
    {
        QMessageBox::warning(nullptr, "error", "hhh", QMessageBox::Yes);
    }

    if (fileServer->listen(QHostAddress::Any, filePort) == false)
    {
        QMessageBox::warning(nullptr, "error", "hhh", QMessageBox::Yes);
    }
}

void Server::stop()
{
    onlineClients.clear();
    fileClients.clear();
    loginClients.clear();
    tcpserver->close();
}

QList<TcpClient*> Server::getOnlineClients()
{
    QList<TcpClient*> result;
    for (auto key : onlineClients.keys())
        result.append(onlineClients.value(key));
    return result;
}

QList<TcpFile*> Server::getFileClients()
{
    QList<TcpFile*> result;
    for (auto key : fileClients.keys())
        result.append(fileClients.value(key));
    return result;
}


void Server::addFileClient(TcpFile* fileClient)
{
    fileClients.insert(fileClient->getUser()->getId(), fileClient);
}


void Server::addOnlineClient(TcpClient *onlineClient)
{
    onlineClients.insert(onlineClient->getUser()->getId(), onlineClient);
}

QList<TcpClient*> Server::getLoginClients()
{
    return loginClients;
}

void Server::addLoginClient(TcpClient *login_client)
{
    loginClients.append(login_client);
}

// 收消息的入口
void Server::receiveMessage(TcpClient* client)
{
    QByteArray message = client->readAll();
    qDebug() << "client message";
    QJsonDocument document = QJsonDocument::fromJson(message);

    //QStringList list = message.split(" ");
    QJsonObject object = document.object();
    int type = object.value("type").toInt();


    switch (type)
    {
    case messageType::LOGIN :
        login(&object, client);
        break;
    case messageType::REGISTER :
        userRegister(&object, client);
        break;
    case messageType::ADDFRIEND :
        addFriend(&object, client);
        break;
    case messageType::SENDMESSAGE :
        sendMessage(&object, client);
        break;
    case messageType::CHATLOG :
        getChatLog(&object, client);
        break;
    case messageType::LOGOUT :
        logout(&object, client);
        break;
    case messageType::ADDFRIENDREPLYINFO :
        addNewFirendReply(&object, client);
        break;
    case messageType::DELETEFRIEND :
        deleteFriend(&object, client);
        break;
    case messageType::GROUPMESSAGE :
        sendGroupMessage(&object, client);
        break;
    default:
        break;
    }
}


// 用户登陆
void Server::login(QJsonObject* object, TcpClient* client)
{
    qDebug() << "login";
    QString account = object->value("account").toString();
    QString password = object->value("password").toString();
    Dao* database = Dao::getInstance();
    User* user = database->getUserByAccountAndPassword(account, password);
    QJsonObject messageObject;
    messageObject.insert("type",messageType::LOGIN);
    if (user == nullptr) {
        qDebug() << "ERROR";
        messageObject.insert("resultStatus", resultStatus::ERROR);
    } else {
        client->setUser(user);
        addOnlineClient(client);
        qDebug() << "SUCCESS";
        messageObject.insert("resultStatus", resultStatus::SUCCESS);
        messageObject.insert("name",user->getName());
        messageObject.insert("id",user->getId());
        messageObject.insert("password",user->getPassword());
        messageObject.insert("account", user->getAccount());
        messageObject.insert("email", user->getEmail());
        messageObject.insert("gender", user->getGender());
        QList<User*> otherUsers = database->getAllUsers();
        QList<User*> friends = database->getFriendUserById(user->getId());
        QList<int> groupIds = database->getGroupIdByUserId(user->getId());



        foreach(User* x, otherUsers)
        {
            foreach(User* y, friends)
            {
                if (x->getId() == y->getId() || x->getId() == user->getId())
                {
                    otherUsers.removeOne(x);
                    break;
                }
            }
        }
        QJsonArray groupList;
        for(int i = 0 ;i < groupIds.size() ; i++){
           QJsonObject group;
           group.insert("groupId",groupIds[i]);
           QList<int> userList = database->getGroupUserIdsByGroupId(groupIds[i]);
           QJsonArray groupUserList ;
           qDebug() << "????";
           for(int j = 0 ; j < userList.size() ; j++){
               qDebug() << "||||" << userList[j];
               QJsonObject userId;
               userId.insert("id",userList[j]);
               groupUserList.insert(j,userId);
           }
           group.insert("groupUserList",groupUserList);
           group.insert("groupName",database->getGroupNameByGroupId(groupIds[i]));
           groupList.insert(i, group);
        }


        messageObject.insert("groupList", groupList);
        QJsonArray friendsList;
        qDebug() << "*********************" << friends.size();
        for(int i = 0 ;i < friends.size() ; i++){
           qDebug() << i << friends[i]->getId() << friends[i]->getName();
           QJsonObject user;
           user.insert("name",friends[i]->getName());
           user.insert("id",friends[i]->getId());
           //user.insert("account", friends[i]->getAccount());
           //user.insert("email", friends[i]->getEmail());
           //user.insert("gender", friends[i]->getGender());
           friendsList.insert(i, user);
        }
        messageObject.insert("friendsList", friendsList);

        QJsonArray otherUsersList;
        qDebug() << "*********************" << otherUsers.size();
        for (int i = 0; i < otherUsers.size(); i++)
        {
            qDebug() << i << otherUsers[i]->getId() << otherUsers[i]->getName();
            QJsonObject user;
            user.insert("name",otherUsers[i]->getName());
            user.insert("id",otherUsers[i]->getId());
            //user.insert("account", otherUsers[i]->getAccount());
            //user.insert("email", otherUsers[i]->getEmail());
            //user.insert("gender", otherUsers[i]->getGender());
            otherUsersList.insert(i, user);
        }
        messageObject.insert("otherUsersList", otherUsersList);
    }

    QJsonDocument document = QJsonDocument(messageObject);
    QByteArray userInfo = document.toJson();// 转换成QByteArray
    client->sendMessage(userInfo);
}

// 用户注册
void Server::userRegister(QJsonObject *object, TcpClient* client)
{
    qDebug() << "register";
    QString account = object->value("account").toString();
    QString name = object->value("name").toString();
    QString password = object->value("password").toString();
    QString email = object->value("email").toString();
    QString gender = object->value("gender").toString();
    //qDebug() << account << name << password << email << gender;
    QJsonObject messageObject;
    messageObject.insert("type", messageType::REGISTER);
    Dao* database = Dao::getInstance();
    bool resultStatus = database->addNewUser(account, name, password, email, gender);
    if (resultStatus == true) {
        messageObject.insert("resultStatus", resultStatus::SUCCESS);
    } else {
        // 找到注册失败的原因
        messageObject.insert("resultStatus", resultStatus::ERROR);
        QList<User*> users = database->getAllUsers();
        foreach(User* user, users)
        {
            if (user->getAccount() == account) {
                messageObject.insert("registerErrorCode", registerErrorCode::ACCOUNT_DUPLICATE);
                break;
            } else if (user->getEmail() == email) {
                messageObject.insert("registerErrorCode", registerErrorCode::EMAIL_DUPLICATE);
                break;
            }
        }
    }
    QJsonDocument document = QJsonDocument(messageObject);
    QByteArray userInfo = document.toJson();// 转换成QByteArray
    client->sendMessage(userInfo);
}

// 添加好友
void Server::addFriend(QJsonObject *object, TcpClient *client)
{
    qDebug() << "addFriend";

    int receiverId = object->value("receiverId").toInt();
    TcpClient* receiverClient = onlineClients.value(receiverId);
    if (receiverClient == nullptr) {
        // 要添加好友的用户不在线
        QJsonObject replyMessageObject;
        replyMessageObject.insert("type", messageType::ADDFRIENDREPLY);
        replyMessageObject.insert("addFriendStatus", addFriendStatus::NO);
        replyMessageObject.insert("addFriendErrorCode", addFriendErrorCode::OFFLINE);
        QJsonDocument document = QJsonDocument(replyMessageObject);
        QByteArray messageInfo = document.toJson();// 转换成QByteArray
        client->sendMessage(messageInfo);
    } else {
        //QJsonObject

        QJsonObject replyMessageObject;
        replyMessageObject.insert("type", messageType::ADDFRIENDREPLY);
        replyMessageObject.insert("addFriendStatus", addFriendStatus::OK);
        QJsonDocument document1 = QJsonDocument(replyMessageObject);
        QByteArray messageInfo1 = document1.toJson();// 转换成QByteArray
        client->sendMessage(messageInfo1);

        QJsonObject addFriendMessageObject;
        addFriendMessageObject.insert("type", messageType::ADDFRIEND);
        addFriendMessageObject.insert("senderId", object->value("senderId").toInt());
        addFriendMessageObject.insert("account", object->value("account").toString());
        addFriendMessageObject.insert("name", object->value("name").toString());
        addFriendMessageObject.insert("receiverId", object->value("receiverId").toInt());
        addFriendMessageObject.insert("verifyInfo", object->value("verifyInfo").toString());
        QJsonDocument document2 = QJsonDocument(addFriendMessageObject);
        QByteArray messageInfo2 = document2.toJson();// 转换成QByteArray
        receiverClient->sendMessage(messageInfo2);
    }


}

void Server::addNewFirendReply(QJsonObject *object, TcpClient *client)
{
    qDebug() << "******************";
    int addFriendReplyInfoStatus = object->value("addFriendReplyInfoStatus").toInt();
    int receiverId = object->value("receiverId").toInt();
    int senderId = object->value("senderId").toInt();
    qDebug() << receiverId << senderId;
    TcpClient* receiverClient = onlineClients.value(senderId);
    if (receiverClient == nullptr)
    {
        qDebug() << "offline";
        return ;
    }


    QJsonObject replyMessageObject;
    replyMessageObject.insert("type", messageType::ADDFRIENDREPLYINFO);
    replyMessageObject.insert("userId", receiverId);
    replyMessageObject.insert("account", object->value("account").toString());
    replyMessageObject.insert("name", object->value("name").toString());
    qDebug() << object->value("account").toString() << object->value("name").toString();
    if (addFriendReplyInfoStatus == addFriendReplyInfoStatus::AGREE) {
        // 添加好友成功
        Dao* database = Dao::getInstance();
        if (database->addNewFriendShip(senderId, receiverId)) {
            replyMessageObject.insert("addFriendReplyInfoStatus", addFriendReplyInfoStatus::AGREE);
            qDebug() << "friendship ok";
        } else {
            replyMessageObject.insert("addFriendReplyInfoStatus", addFriendReplyInfoStatus::UNKNOWN);
            //replyMessageObject.insert("addFriendErrorCode", addFriendErrorCode::UNKNOWN);
            qDebug() << "friendship no1";
        }
    } else {
        // 添加好友失败
        replyMessageObject.insert("addFriendReplyInfoStatus", addFriendReplyInfoStatus::REJECT);
        qDebug() << "friendship no2";
        //replyMessageObject.insert("addFriendErrorCode", addFriendErrorCode::REJECTED);

    }
    QJsonDocument document = QJsonDocument(replyMessageObject);
    QByteArray messageInfo = document.toJson();// 转换成QByteArray
    receiverClient->sendMessage(messageInfo);
}

// 发送消息
void Server::sendMessage(QJsonObject *object, TcpClient* client)
{
    qDebug() << "sendMessage";
    int receiverId = object->value("receiverId").toInt();
    int senderId = client->getUser()->getId();
    TcpClient* receiverClient = onlineClients.value(receiverId);
    // 回复的消息
    QJsonObject replyMessageObject;
    qDebug() << receiverId << senderId << object->value("content").toString();
    //replyMessageObject.insert("returnType", messageType::SENDMESSAGE);
    replyMessageObject.insert("type", messageType::SENDMESSAGE);
    if (receiverClient ==nullptr) {
        // 该好友当前不在线
        replyMessageObject.insert("resultStatus", resultStatus::ERROR);
    } else {
        // 该好友当前在线
        replyMessageObject.insert("resultStatus", resultStatus::SUCCESS);
        // 发送的消息
        QJsonObject messageObject;
        messageObject.insert("type", messageType::RECEIVEMESSAGE);
        messageObject.insert("senderId", senderId);
        QString content = object->value("content").toString();
        QString sendTime = object->value("sendTime").toString();

        messageObject.insert("content", content);
        messageObject.insert("sendTime", sendTime);
        Dao* database = Dao::getInstance();
        bool result = database->saveChatLog(senderId, receiverId, content, sendTime);
        if (result)
            qDebug() << "save success";
        else {
            qDebug() << "save failure";
        }
        QJsonDocument document = QJsonDocument(messageObject);
        QByteArray messageInfo = document.toJson();// 转换成QByteArray
        receiverClient->sendMessage(messageInfo);
    }


    QJsonDocument document = QJsonDocument(replyMessageObject);
    QByteArray replyInfo = document.toJson();// 转换成QByteArray
    client->sendMessage(replyInfo);
}

// 查询聊天记录
void Server::getChatLog(QJsonObject *object, TcpClient *client)
{
    qDebug() << "getChating";
    int senderId = object->value("senderId").toInt();
    int recevierId = object->value("recevierId").toInt();
    Dao* database = Dao::getInstance();
    qDebug() << senderId << recevierId;
    QJsonObject messageObject;
    messageObject.insert("type",messageType::CHATLOG);
    QList<ChatLog*> chatLogList = database->getChatLogByIds(senderId, recevierId);
    User* user = database->getUserById(recevierId);
    // receiverId
    //QList<ChatLog*> chatLogList;
    messageObject.insert("resultStatus", resultStatus::SUCCESS);
    QJsonArray chatLog ;
    for(int i = 0 ;i < chatLogList.size() ; i++) {
       QJsonObject mychatLog;
       mychatLog.insert("id",chatLogList[i]->getId());
       mychatLog.insert("senderId",chatLogList[i]->getSenderId());
       mychatLog.insert("recevierId",chatLogList[i]->getRecevierId());
       mychatLog.insert("content",chatLogList[i]->getContent());
       mychatLog.insert("sendTime",chatLogList[i]->getSendTime());
       mychatLog.insert("otherName", user->getName());
       chatLog.insert(i,mychatLog);
    }
    messageObject.insert("chatLog",chatLog);
    QJsonDocument document=QJsonDocument(messageObject);
    QByteArray userInfo = document.toJson();// 转换成QByteArray
    //qDebug()<<userInfo;
    client->sendMessage(userInfo);
}

void Server::logout(QJsonObject *object, TcpClient* client)
{
    int id = object->value("logoutId").toInt();
    TcpClient* tempClient = onlineClients.value(id);
    if (tempClient != nullptr) {
        qDebug() << "logout success";
        onlineClients.remove(id);
    } else {
        qDebug() << "logout failure";
    }
}

void Server::deleteFriend(QJsonObject *object, TcpClient *client)
{
    int uid = object->value("uid").toInt();
    int fid = object->value("fid").toInt();
    Dao* database = Dao::getInstance();
    database->deleteFriendship(uid, fid);
    TcpClient* userClient = onlineClients.value(fid);
    if (userClient == nullptr)
        return ;
    User* user = client->getUser();

    QJsonObject messageObject;
    messageObject.insert("type", messageType::DELETEFRIENDMESSAGE);
    messageObject.insert("id", uid);
    messageObject.insert("account", user->getAccount());
    messageObject.insert("name", user->getName());

    QJsonDocument document=QJsonDocument(messageObject);
    QByteArray userInfo = document.toJson();// 转换成QByteArray
    qDebug() << userInfo;
    userClient->sendMessage(userInfo);
}

void Server::transmitFile(TcpFile* fileClient)
{
    QByteArray buf = fileClient->readAll();
    qDebug() << buf;
    if (fileClient->isStart) {
        fileClient->isStart = false;
        //解析头部 "head#1024"
        fileClient->senderId = QString(buf).section("#", 0, 0).toInt();
        fileClient->receiverId = QString(buf).section("#", 1, 1).toInt();
        fileClient->fileName = QString(buf).section("#", 2, 2);
        fileClient->fileSize = QString(buf).section("#", 3, 3).toInt();

        fileClient->receiveSize = 0;
        fileClient->targetFileClient = fileClients.value(fileClient->receiverId);
        qDebug() << fileClient->senderId << fileClient->receiverId << fileClient->fileName << fileClient->fileSize;
        if (fileClient->targetFileClient == nullptr)
        {
            qDebug() << "file client not exist error";
            fileClient->isStart = true;
            return ;
        }
        qDebug() << buf;
        fileClient->targetFileClient->write(buf);
        fileClient->targetFileClient->flush();
        QThread::sleep(5);
    } else {
        fileClient->targetFileClient->write(buf);
        fileClient->targetFileClient->flush();
        fileClient->receiveSize += buf.size();
        if(fileClient->receiveSize == fileClient->fileSize)
        {
            qDebug() << "转发文件完成";
            //qDebug() << file.
            fileClient->isStart = true;
            fileClient->fileSize = 0;
            fileClient->receiveSize = 0;
            fileClient->targetFileClient = nullptr;
        }
    }
}

//TODO
void Server::sendGroupMessage(QJsonObject *object, TcpClient *client)
{
    QJsonObject replyMessageObject;
    // senderId groupId 内容
    int senderId = object->value("senderId").toInt();
    int groupId = object->value("groupId").toInt();
    QString content = object->value("content").toString();

    Dao* database = Dao::getInstance();
    User* user = database->getUserById(senderId);
    replyMessageObject.insert("type", messageType::GROUPMESSAGE);
    replyMessageObject.insert("senderId", senderId);
    replyMessageObject.insert("account", user->getAccount());
    replyMessageObject.insert("name", user->getName());
    replyMessageObject.insert("groupId", groupId);
    replyMessageObject.insert("content", content);
    QJsonDocument document = QJsonDocument(replyMessageObject);
    QByteArray message = document.toJson();
    QList<int> userList = database->getGroupUserIdsByGroupId(groupId);
    qDebug() << "group message" << message;
    foreach(int id, userList)
    {
        if (id == senderId)
            continue;
        else {
            foreach(TcpClient* client, onlineClients)
            {
                if (client->getUser()->getId() == id)
                {
                    client->sendMessage(message);
                    break;
                }
            }
        }
        qDebug() << "$$$" << id;
    }
}
