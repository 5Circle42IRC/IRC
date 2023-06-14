#include "../include/IrcClient.hpp"
#include <string>
#include <iostream>

IrcClient::IrcClient(int fd
                    , std::string nickname
                    , std::string password
                    , std::string buffer)
    : _fd(fd)
    , _passwordFlag(false)
    , _nickname(nickname)
    , _password(password)
    , _buffer(buffer)
{ }

IrcClient::IrcClient()
    : _fd(0)
{ }

IrcClient::~IrcClient() {}

IrcClient::IrcClient(const IrcClient &copy)
    : _fd(copy._fd)
    , _passwordFlag(false)
    , _nickname(copy._nickname)
    , _password(copy._password)
    , _buffer(copy._buffer)
{ }

const IrcClient &IrcClient::operator=(const IrcClient &copy)
{
    if (this == &copy)
    {
        this->_fd = copy._fd;
        this->_nickname = copy._nickname;
        this->_password = copy._password;
        this->_buffer = copy._buffer;
        this->_passwordFlag = copy._passwordFlag;
    }
    return (*this);
}

int IrcClient::getFd() const
{
    return _fd;
}

const std::string &IrcClient::getNickname() const
{
    return _nickname;
}

const std::string &IrcClient::getUsername() const
{
    return _username;
}

const std::string &IrcClient::getHostname() const
{
    return _hostname;
}

const std::string &IrcClient::getServername() const
{
    return _servername;
}

const std::string &IrcClient::getRealname() const
{
    return _realname;
}
const std::string &IrcClient::getPassword() const
{
    return _password;
}

const std::string &IrcClient::getBuffer() const
{
    return _buffer;
}

void IrcClient::setNickname(std::string &newNickname)
{
    _nickname = newNickname;
    return;
}

void IrcClient::setUsername(std::string &newUsername)
{
    _username = newUsername;
    return;
}

void IrcClient::setHostname(std::string &newHostname)
{
    _hostname = newHostname;
    return;
}

void IrcClient::setServername(std::string &newServername)
{
    _servername = newServername;
    return;
}

void IrcClient::setRealname(std::string &newRealname)
{
    _realname = newRealname;
    return;
}

void IrcClient::setPassword(std::string &newPassword)
{
    _password = newPassword;
    return;
}

void IrcClient::addBackBuffer(const std::string &str)
{
    _buffer = str;
    return;
}

void IrcClient::addBackCarriageBuffer(const std::string &str)
{
    _buffer = str;
    _buffer = _buffer + "\r\n";
    return;
}

void IrcClient::reduceBuffer(int result)
{
    (void)result;
    return;
}

void IrcClient::setPasswordFlag(bool flag)
{
    _passwordFlag = flag;
}

const bool IrcClient::getPasswordFlag() const
{
    return _passwordFlag;
}

void IrcClient::Display()
{

    std::cout << "------------Display -------" << std::endl;
    std::cout << "Nick : " << getNickname() << std::endl;
    std::cout << "UserName : " << getUsername() << std::endl;
    std::cout << "HostName : " << getHostname() << std::endl;
    std::cout << "ServerName : " << getServername() << std::endl;
    std::cout << "RealName : " << getRealname() << std::endl;
}
