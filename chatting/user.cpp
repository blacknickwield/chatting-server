#include "user.h"

User::User(int _id, QString _name, QString _password):
    id(_id), name(_name), password(_password)
{

}

User::User(int _id, QString _account, QString _name, QString _password, QString _email, QString _gender) :
    id(_id), account(_account), name(_name), password(_password), email(_email), gender(_gender)
{

}

User::User(int _id, QString _account, QString _name, int) :
    id(_id), account(_account), name(_name)
{

}
int User::getId()
{
    return id;
}
QString User::getName()
{
    return name;
}

QString User::getPassword()
{
    return password;
}

QString User::getEmail()
{
    return email;
}

QString User::getGender()
{
    return gender;
}

QString User::getAccount()
{
    return account;
}
