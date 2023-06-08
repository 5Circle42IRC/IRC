#ifndef __IRC_CHANNEL_HPP__
#define __IRC_CHANNEL_HPP__

#include "../InterfaceClass/IChannel.hpp"

class IrcChannel
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
    virtual const bool isOperator(const int clientFd);

    virtual const int getGrant() const;

    virtual const std::string& getTopic() const;
    virtual const std::string& getName() const;
    virtual const std::string& getPassword() const;

private:
    const IrcChannel& operator=(const IrcChannel& copy);
    IrcChannel(const IrcChannel& copy);
    class InvalidGrant {
        public:
            virtual const char *what() const throw();
    };
    class InvalidClientFd : public std::exception {
        virtual const char *what() const throw() { return "invalidClientFd"; }
    };
    std::string _topic;
    std::string _name;
    std::string _password;

    int         _grant;

    std::map<int, bool> _user;
};

#endif