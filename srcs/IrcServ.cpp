#include "../include/Irc.hpp"

#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

IrcServ::IrcServ(int port, std::string passWord)
    : _isError(0)
    , _port(port)
    , _passWord(passWord)
    , _servFd(0)
    , _fdMax(3)
    , _fdNum(0)
    , _opt(1)
{
    std::memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _servAddr.sin_port = htons(port);
    _servFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_servFd == -1)
        Function::errorHandle("socket error", _servFd);

    _isError = setsockopt(_servFd, SOL_SOCKET, SO_REUSEADDR, &_opt, static_cast<socklen_t>(sizeof(_opt)));
    if (_isError)
        Function::errorHandle("setsockopt error", _isError);

    _isError = bind(_servFd, (struct sockaddr*)&_servAddr, sizeof(_servAddr));
    if (_isError)
        Function::errorHandle("fail bind", _isError);

    _isError = listen(_servFd, 5);// 이해  필요.
    if (_isError)
        Function::errorHandle("fail listen", _isError);
}

void IrcServ::run()
{
    FD_ZERO(&_activeReads);
    FD_ZERO(&_activeWrites);
    FD_SET(_servFd, &_activeReads);

    struct timeval  timeout;
    int             acceptFd;

    _fdMax = _servFd;

    while(true) 
    {
        FD_COPY(&_activeReads, &_cpyReads);
        FD_COPY(&_activeWrites, &_cpyWrites);
        // _cpyReads = _activeReads;
        // _cpyWrites = _activeWrites;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100;

        _fdNum = select(_fdMax + 1, &_cpyReads, &_cpyWrites, 0, &timeout);
        if (_fdNum == -1)
            Function::errorHandle("select error", -1);

        char               message[BUFFER_SIZE];
        int                readLen;
        struct sockaddr_in clientAddr;
        socklen_t          clientAddrLen;

        std::memset(message, 0, sizeof(message));

        sleep(1);

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
                    if (acceptFd == -1)
                        continue;
                    if (fcntl(acceptFd, O_NONBLOCK) == -1)
                        Function::errorHandle("fcntl", -1);
                    _clients[acceptFd] = IrcClient(acceptFd, clientAddr, clientAddrLen);
                    FD_SET(acceptFd, &_activeReads);
                    FD_SET(acceptFd, &_activeWrites);
                    if (_fdMax < acceptFd)
                        _fdMax = acceptFd;
                }
                else
                {
                    readLen = read(i, message, BUFFER_SIZE);
                    if (readLen == 0)
                    {
                        FD_CLR(i, &_activeReads);
                        FD_CLR(i, &_activeWrites);
                        close(i);
                        _clients.erase(_clients.find(i));
                    }
                    else
                    {
                        std::cout << message << std::endl; // char??? std::string??? 
                    }
                }
            }
        }
        // std::cout << std::endl;
    }
    close(_servFd);
}

IrcServ::~IrcServ(){};