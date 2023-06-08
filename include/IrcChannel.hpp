#ifndef __IrcCHANNEL__
#define __IrcCHANNEL__

#include "../InterfaceClass/IChannel.hpp"

// class IrcServ;
class IrcClient;

#define MAX_CLIENT_BUFFER_SIZE 1024

class IrcChannel : private IChannel
{
public:
    IrcChannel();
    IrcChannel(std::string channelName);
    virtual ~IrcChannel();
    virtual const bool addUser(const int clientFd);
    virtual const bool deleteUser(const int clientFd, const int target);
    virtual const bool isJoinedUser(const int clientFd);

    virtual void setGrant(typeMode grant, bool on);
    virtual void setOperator(const int clientFd, const int target);
    virtual void setTopic(std::string& newTopic);
    virtual void setPassword(std::string& newPassword);

    virtual const int getGrant() const;
    virtual const bool isOperator(const int clientFd) const;
    virtual const std::string& getTopic() const;
    virtual const std::string& getName() const;
    virtual const std::string& getPassword() const;

protected:
    std::string _topic;
    std::string _name;
    std::string _password;

    int         _grant;

    std::map<int, bool> _user;

private:
    const IrcChannel& operator=(const IrcChannel& copy);
    IrcChannel(const IrcChannel& copy);
    IrcChannel();
};

#endif