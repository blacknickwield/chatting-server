#ifndef CHATLOG_H
#define CHATLOG_H
#include <QString>

class ChatLog
{
public:
    ChatLog(int _id, int _senderId, int _recevierId, QString _content, QString _sendTime);
    int getId();
    int getSenderId();
    int getRecevierId();
    QString getContent();
    QString getSendTime();
private:
    int id;
    int senderId;
    int recevierId;
    QString content;
    QString sendTime;
};

#endif // CHATLOG_H
