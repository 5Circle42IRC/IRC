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

bool IrcServ::acceptClient(int acceptFd, struct sockaddr_in& clientAddr, socklen_t& clientAddrLen, IrcDB& db)
{
    std::memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddrLen = sizeof(clientAddr);
    acceptFd = accept(_servFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (acceptFd == -1 || fcntl(acceptFd, O_NONBLOCK) == -1)
        return false;
    db.insertClient(new IrcClient(acceptFd, "", "", ""));
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

//password 로직, client에 패스워드 불을 넣기
void IrcServ::run()
{
    int acceptFd(0);
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    IrcDB db; //DB선언 DB db;
    IrcCommand command(&db);

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
                    if (!acceptClient(acceptFd, clientAddr, clientAddrLen, db))
                        continue ;
                }
                else
                {
                    memset(_message, 0, sizeof(_message));
                    _readLen = recv(clientFd, _message, BUFFER_SIZE, 0);
                    if (_readLen == -1)
                    {
                        std::cerr << "failed recv" << std::endl;
                        continue;
                    }

                    // std::string test(_message);
                    // std::cerr << "_message : " << _message << std::endl;
                    // std::cerr << "_message len : " << test.length()  << std::endl;
                    // std::cerr << "_message readlen : " << _readLen << std::endl;

                    if (_readLen == 0)
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
                        try {
                            command.setClientFd(clientFd).parsing(_message);
                        } catch (std::exception& e){
                            db.findClientByFd(clientFd)->addBackBuffer(e.what());
                        }
                        std::cout << _message << std::endl; // char??? std::string???
                    }
                }
            }
            else
            {
                if (db.findClientByFd(clientFd)->getBuffer().size() == 0)
                    continue;
                _writeLen = send(clientFd
                                , db.findClientByFd(clientFd)->getBuffer().c_str()
                                , db.findClientByFd(clientFd)->getBuffer().size()
                                , 0);
                db.findClientByFd(clientFd)->reduceBuffer(_writeLen);
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