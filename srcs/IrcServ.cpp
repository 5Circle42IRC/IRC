#include "../include/IrcServ.hpp"
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

IrcServ::IrcServ(){};
IrcServ::IrcServ(int port, std::string passWord)
    : _error(0), _port(port), _passWord(passWord), _servFd(0), _fdMax(3), _fdNum(0), _opt(1)
{ }

int IrcServ::on()
{
    std::memset(_message, 0, sizeof(_message));
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

bool IrcServ::acceptClient(int acceptFd, struct sockaddr_in& clientAddr, socklen_t& clientAddrLen)
{
    std::memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddrLen = sizeof(clientAddr);
    acceptFd = accept(_servFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (acceptFd == -1 || fcntl(acceptFd, O_NONBLOCK) == -1)
        return false;
    send(acceptFd, "input password : ", 17, 0);
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

void IrcServ::run()
{
    int acceptFd(0);
    int readLen(0);
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    //DB선언 DB db;

    while (42)
    {
        initSelect();

        sleep(1);

        for (int clientFd = 0; clientFd < _fdMax + 1; clientFd++)
        {
            // std::cout << clientFd << "debugs" << std::endl;
            if (FD_ISSET(clientFd, &_cpyReads))
            {
                if (clientFd == _servFd)
                {
                    if (!acceptClient(acceptFd, clientAddr, clientAddrLen))
                        continue ;
                }
                else
                {
                    readLen = recv(clientFd, _message, BUFFER_SIZE, 0);

                    if (readLen == 1) 
                        continue ;
                    else if (readLen > 1) 
                        _message[readLen - 1] = '\0';

                    std::string test(_message);
                    std::cerr << "_message : " << _message << std::endl;
                    std::cerr << "_message len : " << test.length()  << std::endl;
                    std::cerr << "_message readlen : " << readLen << std::endl;

                    if (readLen == 0)
                    {
                        deleteClient(clientFd);
                    }
                    else
                    {
                        if (_passWord.compare(_message))
                        {
                            deleteClient(clientFd);
                            continue;
                        }
                        // xcommand자리
                        std::cout << _message << std::endl; // char??? std::string???
                    }
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