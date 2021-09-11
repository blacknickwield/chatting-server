#ifndef DAO_H
#define DAO_H
#include <QString>
#include <QSqlDatabase>
#include <user.h>
#include <QList>
#include "chatlog.h"
class Dao
{
public:
    //查询全部用户
    QList<User*> getAllUsers();

    QList<User*> getFriendUserById(int id);
    // 根据用户名和密码查询用户
    User* getUserByAccountAndPassword(QString account, QString password);

    User* getUserById(int id);
    bool addNewUser(QString account, QString name, QString password, QString email, QString gender);

    bool saveChatLog(int senderId, int recevierId, QString content, QString sendTime);

    bool deleteFriendship(int uid, int fid);

    QList<ChatLog*> getChatLogByIds(int id1, int id2);

    bool addNewFriendShip(int uid, int fid);
    //单例设计模式——应对数据库的对象的内存使用

    QList<int> getGroupIdByUserId(int userId);

    QList<int> getGroupUserIdsByGroupId(int groupId);

    QString getGroupNameByGroupId(int groupId);
    static Dao* getInstance();
private:
    explicit Dao();
    QSqlDatabase db;
    static Dao* database;
    QString driverName = "QMYSQL";
    QString databaseName = "chatting_app";
    QString userName = "root";
    QString userPassword = "toor";
    int port = 3306;
    QString hostName = "localhost";
};

#endif // DAO_H
