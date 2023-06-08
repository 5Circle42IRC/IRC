#ifndef __ICHANNEL_HPP__
#define __ICHANNEL_HPP__

#include <string>
#include <map>

typedef enum 
{
    OPERATOR = 1,
    INVITE = 2,
    TOPIC = 4,
    KEY = 8,
    LIMIT = 16,
}typeMode;

/**
 * 1. canonical form을 지킬 것
 * 2. grant는 비트 마스킹 처음 memset으로 초기화, 비트를 켜려면 | 연산자 사용(i |= B;), 비트를 끄려면  반전 후 &연산자 사용(i &= ~B)
 * 3. user의 key = fd, value = admin여부
*/
class IChannel
{
public:
    virtual const bool addUser(const int clientFd) = 0;
    virtual const bool deleteUser(const int clientFd, const int target) = 0;
    virtual const bool isJoinedUser(const int clientFd) = 0;

    virtual void setGrant(typeMode grant, bool on) = 0;
    virtual void setOperator(const int clientFd, const int target) = 0;
    virtual void setTopic(std::string& newTopic) = 0;
    virtual void setName(std::string& newName) = 0;
    virtual void setPassword(std::string& newPassword) = 0;

    virtual const int getGrant() const = 0;
    virtual const bool isOperator(const int clientFd) const = 0;
    virtual const std::string& getTopic() const = 0;
    virtual const std::string& getName() const = 0;
    virtual const std::string& getPassword() const = 0;

protected:
    std::string _topic;
    std::string _name;
    std::string _password;

    int         _grant;

    std::map<int, bool> _user;
};

#endif