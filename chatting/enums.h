#ifndef ENUMS_H
#define ENUMS_H

#endif // ENUMS_H


// 消息类型
enum messageType
{
    LOGIN=0,
    REGISTER=1,
    ADDFRIEND=2,
    SENDMESSAGE=3,
    RECEIVEMESSAGE = 4,
    CHATLOG=5,
    ADDFRIENDREPLY = 6,
    LOGOUT = 7,
    ADDFRIENDREPLYINFO = 8,
    DELETEFRIEND = 9,
    DELETEFRIENDMESSAGE = 10,
    SENDFILE = 11,
    // test function
    GROUPMESSAGE = 12
};

// 结果状态
enum resultStatus
{
    ERROR=0,
    SUCCESS=1
};

enum registerErrorCode
{
    ACCOUNT_DUPLICATE = 0,
    EMAIL_DUPLICATE = 1,
};

enum loginErrorCode
{

};

enum addFriendErrorCode
{
    OFFLINE = 0,
    REJECTED = 1,
};

enum addFriendStatus
{
    OK = 0,
    NO = 1
};

enum addFriendReplyInfoStatus
{
    REJECT = 0,
    AGREE = 1,
    UNKNOWN = 2
};
