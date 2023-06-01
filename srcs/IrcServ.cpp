#include "../include/IRC.hpp"
#include <arpa/inet.h>
#include <cstring>

IrcServ::IrcServ(int port, std::string passWord)
    : _port(port), _passWord(passWord)
{
    _servFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(_servFd, SOL_SOCKET, SO_REUSEADDR, (const char*)&_servAddr, sizeof(_servAddr));
    if (_servFd == -1) 
        ErrorHandle::errorHandle("socket error", _servFd);
    // setsockopt 추가

    std::memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    _servAddr.sin_port=htons(port);

    if (bind(_servFd, (struct sockaddr*)&_servAddr, sizeof(_servAddr)))
        ErrorHandle::errorHandle("fail bind", -1);
    if (listen(_servFd, 5))
        ErrorHandle::errorHandle("fail listen", -1);
    FD_ZERO(&_activeReads);
    FD_ZERO(&_activeWrites);
}

void IrcServ::run()
{
    struct timeval timeout;

    while(true) 
    {
        FD_SET(_servFd, &_activeReads);
        FD_SET(_servFd, &_activeWrites);

        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        _fdNum = select(_servFd + 1, &_activeReads, &_activeWrites, 0, &timeout);
        if (_fdNum == -1)
            ErrorHandle::errorHandle("select error", -1);

/*
        _fdMax = getFdMax();
        //check
        select(_fdMax, &_activeReads, &_Activewrite, 0, 0);
        selctc하기위한 이이런런저저런  ㅅ작업을하고
        select
        if(FD_ISSET(serv_fd, read_fds))
        {} // 새로운 접속을 처리를 하겠죠.
        for (클라이언트 list를 it로 순환하면서 )
        if(FD_ISSET(client_fd, read_fds))
        클라이언트마다 recv처리하고
            {
                    client_fd를 아니까,
                    client객체깢지 얻을수있어요.
                    kick, invite,등의 명령어를 호출을 할거에요.
                    redv_msg_parse = kick, chaneel, client
                    client객체.채널.kick(client_fd, )
                    kick(client_fd, std::list<string>인자들)

            }
        if(FD_ISSET(client_fd, write_fds))
        send처리.
        */
        std::cout << "IrcServ::run() while loop" << std::endl;
        break;
    }
}

IrcServ::~IrcServ() {};