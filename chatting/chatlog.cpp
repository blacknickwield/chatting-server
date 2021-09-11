#include "chatlog.h"

ChatLog::ChatLog(int _id, int _senderId, int _recevierId, QString _content, QString _sendTime) :
    id(_id), senderId(_senderId), recevierId(_recevierId), content(_content), sendTime(_sendTime)
{

}

int ChatLog::getId()
{
    return id;
}

int ChatLog::getSenderId()
{
    return senderId;
}

int ChatLog::getRecevierId()
{
    return recevierId;
}

QString ChatLog::getContent()
{
    return content;
}

QString ChatLog::getSendTime()
{
    return sendTime;
}
