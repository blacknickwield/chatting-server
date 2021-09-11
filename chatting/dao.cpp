#include "dao.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
Dao* Dao::database = nullptr;

Dao::Dao()
{
    db = QSqlDatabase::addDatabase(driverName);
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(userPassword);
    db.setPort(port);
    if (db.open()) {}
        // qDebug() << "db ok";
    else {
        // qDebug() << "db no";
    }
}

Dao* Dao::getInstance()
{
    if (database == nullptr)
    {
        database = new Dao();
    }
    return database;
}


User* Dao::getUserByAccountAndPassword(QString account, QString password)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE account =:account AND password =:password");
    query.bindValue(":account", QVariant(account));
    query.bindValue(":password", QVariant(password));
    query.exec();
    while (query.next()) {
        QString _account = query.value("account").toString();
        QString _password = query.value("password").toString();
        if (_account == account && _password == password)
            return new User(query.value("id").toInt(),
                            account,
                            query.value("name").toString(),
                            password,
                            query.value("email").toString(),
                            query.value("gender").toString());
    }
    return nullptr;
}

User* Dao::getUserById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE id =:id");
    query.bindValue(":id", QVariant(id));
    query.exec();
    if (query.next())
        return new User(id,
                        query.value("account").toString(),
                        query.value("name").toString(),
                        1);
    return nullptr;
}

QList<User*> Dao::getAllUsers()
{
    QSqlQuery query;
    query.exec("SELECT * FROM user");
    QList<User*> result;
    while (query.next())
        result.append(new User(query.value("id").toInt(),
                              query.value("account").toString(),
                              query.value("name").toString(),
                              query.value("password").toString(),
                              query.value("email").toString(),
                              query.value("gender").toString()));
    return result;
}

QList<User*> Dao::getFriendUserById(int id)
{
    QList<User*> result;
    QSqlQuery query;
    query.prepare("SELECT * FROM friendship, user WHERE (friendship.uid = user.id AND friendship.fid =:id) OR (friendship.fid = user.id AND friendship.uid =:id)");
    query.bindValue(":id", QVariant(id));
    query.exec();
    while (query.next()) {
        result.append(new User(query.value("user.id").toInt(),
                               query.value("user.account").toString(),
                               query.value("user.name").toString(),
                               query.value("user.password").toString(),
                               query.value("user.email").toString(),
                               query.value("user.gender").toString()));
    }
    return result;
}


bool Dao::addNewUser(QString account, QString name, QString password, QString email, QString gender)
{
    QSqlQuery query;
    QString sql = QString("INSERT INTO user (account, name, password, email, gender) VALUES('%1', '%2', '%3', '%4', '%5')")
            .arg(account)
            .arg(name)
            .arg(password)
            .arg(email)
            .arg(gender);
    bool result =  query.exec(sql);
    return result;
}


bool Dao::saveChatLog(int senderId, int recevierId, QString content, QString sendTime)
{
    QSqlQuery query;
    QString sql = QString("INSERT INTO chatlog (senderId, recevierId, content, sendTime) VALUES('%1', '%2', '%3', '%4')")
            .arg(senderId)
            .arg(recevierId)
            .arg(content)
            .arg(sendTime);
    bool result = query.exec(sql);
    return result;
}

QList<ChatLog*> Dao::getChatLogByIds(int id1, int id2)
{
    QList<ChatLog*> result;
    QSqlQuery query;
    query.prepare("SELECT * FROM chatlog WHERE (senderId =:id1 AND recevierId =:id2) OR (senderId =:id2 AND recevierId =:id1) ORDER BY sendTime");
    query.bindValue(":id1", QVariant(id1));
    query.bindValue(":id2", QVariant(id2));
    query.exec();
    while (query.next()) {
        result.append(new ChatLog(query.value("id").toInt(),
                                  query.value("senderId").toInt(),
                                  query.value("recevierId").toInt(),
                                  query.value("content").toString(),
                                  query.value("sendTime").toString()));
    }
    return result;
}

bool Dao::addNewFriendShip(int uid, int fid)
{
    QSqlQuery query;
    QString sql = QString("INSERT INTO friendship (uid, fid) VALUES('%1', '%2')")
            .arg(uid)
            .arg(fid);
    bool result = query.exec(sql);
    return result;
}

bool Dao::deleteFriendship(int uid, int fid)
{
    QSqlQuery query;
    query.prepare("DELETE FROM friendship WHERE (uid =:uid AND fid=:fid) OR (uid =:fid AND fid =:uid)");
    query.bindValue(":uid", QVariant(uid));
    query.bindValue(":fid", QVariant(fid));
    bool result = query.exec();
    return result;
}

QList<int> Dao::getGroupIdByUserId(int userId)
{
    QList<int> result;
    QSqlQuery query;
    query.prepare("SELECT * FROM grouprelationship WHERE userId =:userId");
    query.bindValue(":userId", QVariant(userId));
    query.exec();
    while (query.next()) {
        result.append(query.value("groupId").toInt());
    }
    return result;
}

QList<int> Dao::getGroupUserIdsByGroupId(int groupId)
{
    QList<int> result;
    QSqlQuery query;
    query.prepare("SELECT * FROM grouprelationship WHERE groupId =:groupId");
    query.bindValue(":groupId", QVariant(groupId));
    query.exec();
    while (query.next()) {
        result.append(query.value("userId").toInt());
    }
    return result;
}

QString Dao::getGroupNameByGroupId(int groupId)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM groupchat WHERE id =:groupId");
    query.bindValue(":groupId", QVariant(groupId));
    query.exec();
    if (query.next())
        return query.value("name").toString();
    return nullptr;
}
