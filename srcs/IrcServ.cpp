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

bool IrcServ::setSelect()
{
    _cpyReads = _activeReads;
    _cpyWrites = _activeWrites;
    _timeout.tv_sec = 0;
    _timeout.tv_usec = 100;

    _fdNum = select(_fdMax + 1, &_cpyReads, &_cpyWrites, 0, &_timeout);
    if (_fdNum == -1)
        return true;
    return false;
}



int IrcServ::response()
{
    // FD_COPY(&_activeReads, &_cpyReads);
    // FD_COPY(&_activeWrites, &_cpyWrites);
    int readLen;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;

    std::memset(message, 0, sizeof(message));
    int len;

    sleep(1);//

    for (int i = 0; i < _fdMax + 1; i++)
    {
        // std::cout << i << "debugs" << std::endl;
        if (FD_ISSET(i, &_cpyReads))
        {
            if (i == _servFd)
            {
                std::memset(&clientAddr, 0, sizeof(clientAddr));
                clientAddrLen = sizeof(clientAddr);
                acceptFd = accept(_servFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
                if (acceptFd == -1 || fcntl(acceptFd, O_NONBLOCK) == -1)
                    continue;
                send(acceptFd, "input password :", 16, 0);
                FD_SET(acceptFd, &_activeReads);
                FD_SET(acceptFd, &_activeWrites);
                if (_fdMax < acceptFd)
                    _fdMax = acceptFd;
            }
            else
            {
                readLen = recv(i, _message, BUFFER_SIZE, 0);

                if (readLen == 1) continue;
                else if (readLen > 1) _message[readLen - 1] = '\0';

                std::string test(_message);
                std::cerr << "_message : " << _message << std::endl;
                std::cerr << "_message len : " << test.length()  << std::endl;
                std::cerr << "_message readlen : " << readLen << std::endl;

                if (readLen == 0)
                {
                    FD_CLR(i, &_activeReads);
                    FD_CLR(i, &_activeWrites);
                    close(i);
                    try {
                        for (std::map<std::string, IrcChannel&>::iterator it = _channel.begin(); it != _channel.end(); i++)
                            it->second.deleteUser(i);
                        _client.erase(i);
                    } catch(const std::exception& e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
                else
                {
                    try {
                        client = find(i);
                    } catch(const std::exception& e) {
                        if (_passWord.compare(_message))
                        {
                            FD_CLR(i, &_activeReads);
                            FD_CLR(i, &_activeWrites);
                            close(i);
                            continue;
                        }
                        registerClient(acceptFd);
                        continue;
                    }
                    len = send(i, client.getBuffer().c_str(), client.getBuffer().length(), 0);
                    if (len == -1)
                        continue;
                    client.reduceBuffer(len);
                    // send _message : set nickname set password
                    std::cout << _message << std::endl; // char??? std::string???
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