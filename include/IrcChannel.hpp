#ifndef __IRC_CHANNEL_HPP__
#define __IRC_CHANNEL_HPP__

#include <map>
#include <iostream>

typedef enum 
{
    M_INVITE = 2,
    M_TOPIC = 4,
    M_KEY = 8,
    M_LIMIT = 16,
}typeMode;

class IrcChannel
{
public:
    IrcChannel();
    IrcChannel(std::string channelName);
    const IrcChannel& operator=(const IrcChannel& copy);
    ~IrcChannel();
    void addUser(const int clientFd);
    bool deleteUser(const int target);
    bool isJoinedUser(const int clientFd) const;

    void setGrant(typeMode grant, bool on);
    void setOperator(const int clientFd, const int target);
    void setTopic(std::string& newTopic);
    void setPassword(std::string& newPassword);
    bool isOperator(const int clientFd) const;

    int getGrant() const;

    const std::string&          getTopic() const;
    std::string&                getName();
    const std::string&          getPassword() const;
    const std::map<int, bool>&  getUser() const;
    const int                   getLimit() const;

protected:
    IrcChannel(const IrcChannel& copy);
    std::string         _name;
    std::string         _topic;
    std::string         _password;

    int					_grant;
    int                 _limit;

    std::map<int, bool>	_user;

private:

    class InvalidGrant : public std::exception {
        public:
            virtual const char *what() const throw();
    };
    class InvalidClientFd : public std::exception {
        virtual const char *what() const throw() { return "invalidClientFd"; }
    };
};

#endif