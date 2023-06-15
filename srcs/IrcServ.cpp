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
    // 여기는 삽입하지 않음?
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
                switch (static_cast<int>(clientFd == _servFd))
                {
                case ENTER_CLIENT:
                    if (!acceptClient(acceptFd, clientAddr, clientAddrLen, db))
                        std::cerr << "failed accept" << std::endl;
                    break;
                default:
                    // 여기 문제가 있음... 왜 안찾아짐?
                    IrcClient *clientClass = db.findClientByFd(clientFd);

                    memset(_recvMessage, 0, sizeof(_recvMessage));
                    _readLen = recv(clientFd, _recvMessage, BUFFER_SIZE, 0);
                    if (clientClass->getPasswordFlag() && _readLen == -1)
                    {
                        std::cerr << "failed recv" << std::endl;
                        break;
                    }

                    std::string test(_recvMessage);
                    std::cerr << "_recvMessage : " << _recvMessage << std::endl;
                    std::cerr << "_recvMessage len : " << test.length()  << std::endl;
                    std::cerr << "_recvMessage readlen : " << _readLen << std::endl;

                    switch (_readLen)
                    {
                    case EXIT_CLIENT:
                        std::cout << "exit_Client" << std::endl;
                        deleteClient(clientFd);
                        break;
                    default:
                        //password로직?
                        if (clientClass->getPasswordFlag() == false)
                        {
                            if (!_passWord.compare(_recvMessage))
                            {
                                clientClass->setPasswordFlag(true);
                                if (send(clientFd, "set your password : ", 19, 0))
                                    std::cerr << "not send" << std::endl;
                                std::cerr << "acceptFd : " << acceptFd << std::endl;
                                std::cerr << "clientFd : " << clientFd << std::endl;
                            } else {
                                _sendMessage = "Failed Password, plz connecting again";
                                send(clientFd, _sendMessage.c_str(), _sendMessage.length(), 0);
                                deleteClient(clientFd);
                            }
                        }
                        else if (clientClass->getPassword().length() == EMPTY)
                        {
                            clientClass->setPassword(_recvMessage);
                            send(clientFd, "input nickname : ", 17, 0);
                        } else if (clientClass->getNickname().length() == EMPTY) {
                            clientClass->setNickname(_recvMessage);
                            send(clientFd, "input realname : ", 17, 0);
                        } else if (clientClass->getUsername().length() == EMPTY) {
                            clientClass->setUsername(_recvMessage);
                            _sendMessage = "$> ";
                            send(clientFd, _sendMessage.c_str(), _sendMessage.length(), 0);
                        } else {
                            _sendMessage = "$> ";
                            send(clientFd, _sendMessage.c_str(), _sendMessage.length(), 0);
                            try {
                                command.setClientFd(clientFd).parsing(_recvMessage);
                            } catch (std::exception& e){
                                clientClass->addBackBuffer(e.what());
                            }
                            std::cout << _recvMessage << std::endl; // char??? std::string???
                        }
                        break;
                    }

                    if (clientClass->getBuffer().size() == 0)
                        break;
                    _writeLen = send(clientFd
                                    , clientClass->getBuffer().c_str()
                                    , clientClass->getBuffer().size()
                                    , 0);
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