#ifndef USER_H
#define USER_H
#include <QString>

class User
{
public:
    User(int id, QString name, QString password);
    User(int id, QString account, QString name, QString password, QString email, QString gender);
    User(int id, QString account, QString name, int);
    int getId();
    QString getName();
    QString getPassword();
    QString getAccount();
    QString getEmail();
    QString getGender();

private:
    int id;
    QString account;
    QString name;
    QString password;
    QString email;
    QString gender;

};

#endif // USER_H
