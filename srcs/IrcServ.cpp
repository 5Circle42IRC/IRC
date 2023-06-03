#include "../include/IRC.hpp"
#include <arpa/inet.h>
#include <cstring>

IrcServ::IrcServ(int port, std::string passWord)
    : _port(port)
    , _passWord(passWord)
    , _servFd(0)
    , _fdMax(0)
    , _fdNum(0)
    , _opt(true)
    , _bsize(0)
    , _isError(0)
{
    _servFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_servFd == -1)
        ErrorHandle::errorHandle("socket error", _servFd);

    _isError = setsockopt(_servFd, SOL_SOCKET, SO_REUSEADDR, &_opt, _bsize);
    if (_isError)
        ErrorHandle::errorHandle("setsockopt error", _isError);

    std::memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _servAddr.sin_port=htons(port);

    _isError = bind(_servFd, (struct sockaddr*)&_servAddr, sizeof(_servAddr));
    if (_isError)
        ErrorHandle::errorHandle("fail bind", _isError);

    _isError = listen(_servFd, 5);
    if (_isError)
        ErrorHandle::errorHandle("fail listen", _isError);
}

void IrcServ::run()
{
    struct timeval timeout;
    int acceptFd;

    FD_ZERO(&_activeReads);
    FD_ZERO(&_activeWrites);

    FD_SET(_servFd, &_activeReads);
    FD_SET(_servFd, &_activeWrites);

    while(true) 
    {
        _cpyReads = _activeReads;
        _cpyWrites = _activeWrites;
        
        timeout.tv_sec = 0;
        timeout.tv_usec = 100;
        _fdMax = select(_servFd + 1, &_cpyReads, &_cpyWrites, 0, &timeout);
        if (_fdMax == 0)
            continue;
        else if (_fdMax == -1)
            ErrorHandle::errorHandle("select error", -1);
        for (int i = 0; i < _fdMax + 1; i++)
        {
            if (FD_ISSET(i, &_cpyReads))
            {
                if (i == _servFd)
                {
                    acceptFd = accept(_servFd, (struct sockaddr *)&_servAddr, &_bsize);
                    if (acceptFd == -1)
                        continue;
                    FD_SET(acceptFd, &_activeReads);
                    FD_SET(acceptFd, &_activeWrites);
                    _clients[acceptFd] = IrcClient(acceptFd, htonl(_servAddr.sin_addr.s_addr), htons(_servAddr.sin_port));
                }
                else
                {
                    
                }
            }
        }
        std::cout << "IrcServ::run() while loop" << std::endl;
        break; //
    }
}

IrcServ::~IrcServ() {};