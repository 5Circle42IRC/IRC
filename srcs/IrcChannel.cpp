#include "../include/IrcChannel.hpp"

IrcChannel::IrcChannel() : _name("no Name"), _topic("any thing") {}
 
IrcChannel::IrcChannel(std::string name)
    : _name(name), _topic("any thing") {}

IrcChannel::~IrcChannel() {}

//addUser시 이미 존재하는 user는 add하지 않음
const bool IrcChannel::addUser(const int clientFd) {
    if (isJoinedUser(clientFd)){
	    _user[clientFd] = false;
        return true;
    }
    return false;
}
const bool IrcChannel::deleteUser(const int clientFd, const int target) {
    if (isOperator(clientFd)){
        _user.erase(target);
        return true;
    }
    return false;
}
const bool IrcChannel::isJoinedUser(const int clientFd) {
    return _user.count(clientFd) != 0;
}

void IrcChannel::setGrant(typeMode grant, bool on) {
    if (on){
        _grant |= grant;
        std::cout << grant << " mode is set" << std::endl;
    }
    else
        _grant &= ~grant;
}

void IrcChannel::setOperator(const int clientFd, const int target) {
    if (isOperator(clientFd))
        _user[target] = true;
}
void IrcChannel::setTopic(std::string& newTopic) { _topic = newTopic; }
void IrcChannel::setPassword(std::string& newPassword) { _password = newPassword; }

const int IrcChannel::getGrant() const { return _grant; }
const bool IrcChannel::isOperator(const int clientFd){
    if (isJoinedUser(clientFd))
        return static_cast<const bool>(_user[clientFd]);
    throw InvalidClientFd();
}

const std::string& IrcChannel::getTopic() const { return _topic; }
const std::string& IrcChannel::getName() const { return _name; }
const std::string& IrcChannel::getPassword() const { return _password; }