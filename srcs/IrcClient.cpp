#include "../include/IrcClient.hpp"
#include <string>

IrcClient::IrcClient(int fd, std::string nickname, std::string password, std::string buffer)
    : _fd(fd), _nickname(nickname), _password(password), _buffer(buffer)
{ }

IrcClient::IrcClient()
    : _fd(0)
{ }

IrcClient::~IrcClient() { }

IrcClient::IrcClient(const IrcClient& copy)
    : _fd(copy._fd), _nickname(copy._nickname), _password(copy._password), _buffer(copy._buffer)
{ }

const IrcClient& IrcClient::operator=(const IrcClient& copy)
{
    if (this == &copy)
    {
        this->_fd = copy._fd;
        this->_nickname = copy._nickname;
        this->_password = copy._password;
        this->_buffer = copy._buffer;
    }
    return (*this);
}

int			IrcClient::getFd() const
{
    return _fd;
}

const std::string&	IrcClient::getNickname() const
{
    return _nickname;
}

const std::string&	IrcClient::getPassword() const
{
    return _password;
}

const std::string&	IrcClient::getBuffer() const
{
    return _buffer;
}

void		IrcClient::setNickname(std::string& newNickname)
{
    _nickname = newNickname;
    return ;
}

void IrcClient::setUsername(std::string &newUsername)
{
    _username = newUsername;
    return ;
}

void IrcClient::setHostname(std::string &newHostname)
{
    _hostname = newHostname;
    return ;
}

void IrcClient::setServername(std::string &newServername)
{
    _servername = newServername;
    return ;
}

void IrcClient::setRealname(std::string &newRealname)
{
    _realname = newRealname;
    return ;
}

void		IrcClient::setPassword(std::string& newPassword)
{
    _password = newPassword;
    return ;
}

void		IrcClient::addBackBuffer(std::string& str)
{
    (void)str;
    return ;
}

void		IrcClient::reduceBuffer(int result)
{
    (void)result;
    return ;
}

/*
void IrcClient::Display() {


    char ipAddrStr1[16];
    char ipAddrStr2[16];
    
    printf ("Irc Client : [%s , %d] connected to [%s, %d]\n", 
        network_convert_ip_n_to_p(this->ip_addr, ip_addr_str1),
        (this->port_no),
        network_convert_ip_n_to_p(this->server_ip_addr, ip_addr_str2),
        (this->server_port_no)
        );
}
 */
