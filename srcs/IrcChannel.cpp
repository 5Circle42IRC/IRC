#include "../include/IrcChannel.hpp"

IrcChannel::IrcChannel() : _name("no Name"), _topic("any thing") {}
 
IrcChannel::IrcChannel(std::string name)
    : _name(name), _topic("any thing") {}

IrcChannel::~IrcChannel() {}

const IrcChannel& IrcChannel::operator=(const IrcChannel& copy){
    if (this == &copy)
        return *this;
    _topic = copy._topic;
    _name = copy._name;
    _password = copy._password;
    _grant = copy._grant;
    _user = copy._user;
}

//addUser시 이미 존재하는 user는 add하지 않음
const void IrcChannel::addUser(const int clientFd) {
	_user[clientFd] = false;
}
const void IrcChannel::deleteUser(const int target) {
    _user.erase(target);
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