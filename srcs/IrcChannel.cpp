// #include <arpa/inet.h>
// #include "../include/Irc.hpp"

#include "../include/IrcChannel.hpp"

IrcChannel::IrcChannel() 
    : _name("no Name"), _topic("any thing") {}

IrcChannel::IrcChannel(std::string name)
    : _name(name), _topic("any thing") {}

IrcChannel::~IrcChannel() {}


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
}

void IrcChannel::setOperator(const int clientFd, const int target) {}
void IrcChannel::setTopic(std::string& newTopic) { _topic = newTopic; }
void IrcChannel::setPassword(std::string& newPassword) { _password = newPassword; }

const int IrcChannel::getGrant() const { return _grant; }
const bool IrcChannel::isOperator(const int clientFd) const {
    try {
        int result;
        resutl = _user.find(clientFd);
}
const std::string& IrcChannel::getTopic() const {}
const std::string& IrcChannel::getName() const {}
const std::string& IrcChannel::getPassword() const {}