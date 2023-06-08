<<<<<<< HEAD
#include <arpa/inet.h>
#include "../include/Irc.hpp"

IrcChannel::IrcChannel() 
    : _channelName("no Name"), _channelTopic("any thing")
{ }

IrcChannel::IrcChannel(std::string channelName)
    : _channelName(channelName), _channelTopic("any thing")
{ }

IrcChannel::~IrcChannel() {};

IrcChannel::IrcChannel(const IrcChannel& copy)
    : _channelName(copy._channelName), _channelTopic(copy._channelTopic)
{
    if (this != &copy)
        throw CopyError();
}

const IrcChannel& IrcChannel::operator=(const IrcChannel& copy)
{
    if (this != &copy)
    {
        _channelName = copy._channelName;
        _channelTopic = copy._channelTopic;
    }
    return *this;
=======
// #include <arpa/inet.h>
// #include "../include/Irc.hpp"

#include "../include/IrcClient.hpp"

IrcChannel::IrcChannel() 
    : _name("no Name"), _topic("any thing") {}

IrcChannel::IrcChannel(std::string name)
    : _name(name), _topic("any thing") {}

IrcChannel::~IrcChannel() {};

IrcChannel::IrcChannel(const IrcChannel& copy)
    : _name(copy._name), _topic(copy._topic)
{
    if (this != &copy)
        throw CopyError();
}

const IrcChannel& IrcChannel::operator=(const IrcChannel& copy)
{
    if (this != &copy)
    {
        _name = copy._name;
        _topic = copy._topic;
    }
    return *this;
}

const bool IrcChannel::addUser(const int clientFd) {}
const bool IrcChannel::deleteUser(const int clientFd, const int target) {}
const bool IrcChannel::isJoinedUser(const int clientFd) {}

void IrcChannel::setGrant(typeMode grant, bool on) {
    if (on){
        _grant |= grant;
        std::cout << grant << " mode is set" << std::endl;
    }
    else
        _grant &= ~grant;
>>>>>>> 37ddfd7eb53da24f168babb4e1f4ca82c1175692
}

void IrcChannel::setOperator(const int clientFd, const int target) {}
void IrcChannel::setTopic(std::string& newTopic) { _topic = newTopic; }
void IrcChannel::setPassword(std::string& newPassword) { _password = newPassword; }

const int IrcChannel::getGrant() const {}
const bool IrcChannel::isOperator(const int clientFd) const {}
const std::string& IrcChannel::getTopic() const {}
const std::string& IrcChannel::getName() const {}
const std::string& IrcChannel::getPassword() const {}