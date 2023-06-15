#include "../include/IrcServ.hpp"
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>

IrcServ::IrcServ(){};
IrcServ::IrcServ(int port, std::string passWord)
    : _error(0)
    , _port(port)
    , _passWord(passWord)
    , _servFd(0)
    , _fdMax(3)
    , _fdNum(0)
    , _opt(1)
    , _readLen(0)
    , _writeLen(0)
{ }

int IrcServ::on()
{
    _passWord.append("\n");
    std::memset(_recvMessage, 0, sizeof(_recvMessage));
    std::memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _servAddr.sin_port = htons(_port);
    _servFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_servFd == -1)
        throw IrcServ::socketException();

    _error = fcntl(_servFd, O_NONBLOCK);
    if (_error == -1)
        throw IrcServ::fcntlException();

    _error = setsockopt(_servFd, SOL_SOCKET, SO_REUSEADDR, &_opt, static_cast<socklen_t>(sizeof(_opt)));
    if (_error)
        throw IrcServ::setsockoptException();

    _error = bind(_servFd, (struct sockaddr *)&_servAddr, sizeof(_servAddr));
    if (_error)
        throw IrcServ::bindException();

    _error = listen(_servFd, 5); // 이해  필요.
    if (_error)
        throw IrcServ::listenException();
    FD_ZERO(&_activeReads);
    FD_ZERO(&_activeWrites);
    FD_SET(_servFd, &_activeReads);
    _fdMax = _servFd;
    return _servFd;
}

bool IrcServ::initSelect()
{
    // FD_COPY(&_activeReads, &_cpyReads);
    // FD_COPY(&_activeWrites, &_cpyWrites);
    _cpyReads = _activeReads;
    _cpyWrites = _activeWrites;
    _timeout.tv_sec = 0;
    _timeout.tv_usec = 100;

    _fdNum = select(_fdMax + 1, &_cpyReads, &_cpyWrites, 0, &_timeout);
    if (_fdNum == -1)
        return true;
    return false;
}

bool IrcServ::acceptClient(int acceptFd, struct sockaddr_in& clientAddr, socklen_t& clientAddrLen, IrcDB& db)
{
    std::memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddrLen = sizeof(clientAddr);
    acceptFd = accept(_servFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (acceptFd == -1 || fcntl(acceptFd, O_NONBLOCK) == -1)
        return false;
    _sendMessage = "input server password : ";
    send(acceptFd, _sendMessage.c_str(), _sendMessage.length(), 0);
    db.insertClient(new IrcClient(acceptFd, "", "", ""));
    FD_SET(acceptFd, &_activeReads);
    FD_SET(acceptFd, &_activeWrites);
    if (_fdMax < acceptFd)
        _fdMax = acceptFd;
    return true;
}

void IrcServ::deleteClient(int fd)
{
    FD_CLR(fd, &_activeReads);
    FD_CLR(fd, &_activeWrites);
    close(fd);
}

void IrcServ::displayServerParam(const int clientFd, const IrcDB& db)
{
	std::map<std::string , IrcChannel*> channels(db.getAllChannels());
	std::map<int, IrcClient *> clients(db.getAllClients());
    
    std::cout << "\033[31m" << "--------------------------------------" << std::endl;
    for(std::map<int, IrcClient *>::iterator clientIt = clients.begin(); clientIt != clients.end(); clientIt++)
    {
        std::cerr << "client fd : " << clientIt->first << std::endl;
        std::cerr << "client username : " << "[" << clientIt->second->getUsername() << "]" << std::endl;
        std::cerr << "client nickname : " << "[" << clientIt->second->getNickname() << "]" << std::endl;
        std::cerr << "client password : " << "[" << clientIt->second->getPassword() << "]" << std::endl;
    }

    std::cout << "\n======================================" << std::endl;
    std::cerr << "channl all size : " << "[" << channels.size() << "]" << std::endl;
    for (std::map<std::string, IrcChannel*>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::cout << "\n=-=-=-=-=-=-=-=-=-=" << "[ "  
                  << "channel name : " << it->second->getName() << " ]" 
                  << "-=-=-=-=-=-=-=-=-=-" << std::endl;
        std::cerr << "channel password : " << "[" << it->second->getPassword() << "]" << std::endl;
        std::cerr << "channel limit : " << "[" << it->second->getLimit() << "]" << std::endl;
        std::cerr << "channel grant : " << "[" << it->second->getGrant() << "]" << std::endl;
        std::cerr << "channel topic : " << "[" << it->second->getTopic() << "]" << std::endl;
    }

    std::cout << "\n======================================" << std::endl;
    std::cerr << "server recieve Message : " << "[" << _recvMessage << "]" << std::endl;
    std::cerr << "server recieve Message len : " << std::strlen(_recvMessage) << std::endl;
    std::cout << "--------------------------------------" << "\033[0m" << std::endl;
}

void IrcServ::run()
{
    int acceptFd(0);
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    IrcDB db; 
    IrcCommand command(&db);
    int messageLen(0);

    while (42)
    {
        initSelect();
        for (int clientFd = 0; clientFd < _fdMax + 1; clientFd++)
        {
            if (FD_ISSET(clientFd, &_cpyReads))
            {
                switch (static_cast<int>(clientFd == _servFd))
                {
                case ENTER_CLIENT:
                    if (!acceptClient(acceptFd, clientAddr, clientAddrLen, db))
                        std::cerr << "failed accept" << std::endl;
                    break;
                default:
                    IrcClient *clientClass = db.findClientByFd(clientFd);

                    memset(_recvMessage, 0, sizeof(_recvMessage));
                    _readLen = recv(clientFd, _recvMessage, BUFFER_SIZE, 0);
                    if (clientClass->getPasswordFlag() && _readLen == -1)
                    {
                        std::cerr << "failed recv" << std::endl;
                        break;
                    }

                    switch (_readLen)
                    {
                    case EXIT_CLIENT:
                        std::cout << "exit_Client" << std::endl;
                        deleteClient(clientFd);
                        break;
                    default:
                        messageLen = std::strlen(_recvMessage);
                        if (clientClass->getPasswordFlag() == false) {
                            if (!_passWord.compare(_recvMessage))
                            {
                                clientClass->setPasswordFlag(true);
                                send(clientFd, "input nickname : ", 17, 0);
                            } else {
                                _sendMessage = "Failed Password, plz connecting again";
                                send(clientFd, _sendMessage.c_str(), _sendMessage.length(), 0);
                                deleteClient(clientFd);
                            }
                        } else if (clientClass->getNickname().length() == EMPTY) {
                            if (messageLen < 2|| messageLen > 10) {
                                _sendMessage = "wrong input retry : ";
                                send(clientFd, _sendMessage.c_str(), _sendMessage.length(), 0);
                            } else {
                                send(clientFd, "set your password : ", 20, 0);
                                _recvMessage[messageLen - 1] = '\0';
                                clientClass->setNickname(_recvMessage);
                            }
                        } else if (clientClass->getPassword().length() == EMPTY) {
                            if (messageLen < 2) {
                                _sendMessage = "wrong input retry : ";
                                send(clientFd, _sendMessage.c_str(), _sendMessage.length(), 0);
                            } else {
                                _recvMessage[messageLen - 1] = '\0';
                                // 여기서 닉네임이 동일한지 판단.
                                clientClass->setPassword(_recvMessage);
                                send(clientFd, "input realname : ", 17, 0);
                            }
                        } else if (clientClass->getUsername().length() == EMPTY) {
                            if (messageLen < 2) {
                                _sendMessage = "wrong input retry : ";
                                send(clientFd, _sendMessage.c_str(), _sendMessage.length(), 0);
                            } else {
                                _recvMessage[messageLen - 1] = '\0';
                                clientClass->setUsername(_recvMessage);
                            }
                        } else {
                            try {
                                command.setClientFd(clientFd).parsing(_recvMessage);
                            } catch (std::exception& e){
                                clientClass->addBackBuffer(e.what());
                            }
                        }
                        break;
                    }
                    if (clientClass->getBuffer().size() == 0)
                        break;
                    displayServerParam(clientFd, db);
                    _writeLen = send(clientFd
                                    , clientClass->getBuffer().c_str()
                                    , clientClass->getBuffer().size()
                                    , 0);
                    if (_writeLen > 0)
                        std::cerr << "\033[38;5;3m$> user "<< clientClass->getNickname() 
                                  << "-------recieve massage------\n" 
                                  << clientClass->getBuffer() << "\033[0m"
                                  << "-------recieve massage------\n" << std::endl;
                    clientClass->reduceBuffer(_writeLen);
                    break;
                }
            }
        }
    }
}

const char *IrcServ::socketException::what() const throw()
{
    return "socket error";
}

const char *IrcServ::fcntlException::what() const throw()
{
    return "fcntl error";
}

const char *IrcServ::setsockoptException::what() const throw()
{
    return "setsockopt error";
}

const char *IrcServ::bindException::what() const throw()
{
    return "bind error";
}

const char *IrcServ::listenException::what() const throw()
{
    return "listen error";
}

const char *IrcServ::selectException::what() const throw()
{
    return "select error";
}

const char *IrcServ::acceptException::what() const throw()
{
    return "accept error";
}

IrcServ::~IrcServ(){};