/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:46:28 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/26 13:16:01 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/IrcServ.hpp"
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>

IrcServ::IrcServ(){};
IrcServ::~IrcServ(){};
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
    std::memset(_recvMessage, 0, sizeof(_recvMessage));
    std::memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _servAddr.sin_port = htons(_port);
    _servFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_servFd == -1)
        throw IrcServ::socketException();

    _error = fcntl(_servFd, F_SETFL, O_NONBLOCK);
    if (_error == -1)
        throw IrcServ::fcntlException();

    _error = setsockopt(_servFd, SOL_SOCKET, SO_REUSEADDR, &_opt, static_cast<socklen_t>(sizeof(_opt)));
    if (_error)
        throw IrcServ::setsockoptException();

    _error = bind(_servFd, (struct sockaddr *)&_servAddr, sizeof(_servAddr));
    if (_error)
        throw IrcServ::bindException();

    _error = listen(_servFd, 5);
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
    FD_COPY(&_activeReads, &_cpyReads);
    FD_COPY(&_activeWrites, &_cpyWrites);
    _timeout.tv_sec = 0;
    _timeout.tv_usec = 0;

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
    if (acceptFd == -1 || fcntl(acceptFd, F_SETFL, O_NONBLOCK) == -1)
        return false;
    
    db.insertClient(new IrcClient(acceptFd, "", "", ""));
    db.findClientByFd(acceptFd)->addBackCarriageBuffer("input password");
    FD_SET(acceptFd, &_activeReads);
    if (_fdMax < acceptFd)
        _fdMax = acceptFd;
    return true;
}

void IrcServ::deleteClient(int clientFd, IrcDB& db)
{
    std::map<std::string , IrcChannel*>_channels = db.getAllChannels();
    std::map<std::string , IrcChannel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); it++)
    {
        if (it->second->isJoinedUser(clientFd))
            it->second->deleteUser(clientFd);
        if (it->second->getUser().size() == 0)
            db.deleteChannel(it->second->getName());
    }    
    db.deleteClient(clientFd);
    FD_CLR(clientFd, &_activeReads);
    close(clientFd);
}

bool IrcServ::isSameNickname(IrcDB& db, std::string message)
{
    std::map<int, IrcClient *> clients(db.getAllClients());

    for(std::map<int, IrcClient *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (!it->second->getNickname().compare(message))
            return true;
    }
    return false;
}

void IrcServ::excuteCommand(IrcCommand& command, const int clientFd, IrcClient* clientClass)
{
    try {
            command.setClientFd(clientFd).parsing(clientClass->getNextLineReadBuffer());
            clientClass->reduceReadBuffer(clientClass->getNextLineReadBuffer().size() + 1);

    } catch (std::exception& e){
    } catch (...) {
        
    }
}

void IrcServ::writeUserBuffer(const int clientFd, IrcClient* clientClass)
{
    if (clientClass->getBuffer().size())
    {
        _sendMessage = clientClass->getBuffer();
        _writeLen = send(clientFd
                        , _sendMessage.c_str()
                        , _sendMessage.size()
                        , 0);
        if (_writeLen > 0)
        {
        }
        FD_CLR(clientFd, &_activeWrites);
        clientClass->reduceBuffer(_writeLen);
    }
}

void IrcServ::run()
{
    int acceptFd(0);
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    IrcDB db; 
    IrcClient *clientClass;

    db.setServPass(_passWord);

    while (42)
    {
        initSelect();
        for (int clientFd = 0; clientFd < _fdMax + 1; clientFd++)
        {
            try {
                clientClass = db.findClientByFd(clientFd);
                if (clientClass->getBuffer().length() != EMPTY)
                {
                    FD_SET(clientFd, &_activeWrites);
                }
            } catch (std::exception& e) {
            } catch (...) { }

            if (FD_ISSET(clientFd, &_cpyReads))
            {
                switch (static_cast<int>(clientFd == _servFd))
                {
                case ENTER_CLIENT:        
                    if (!acceptClient(acceptFd, clientAddr, clientAddrLen, db))
                    {}
                    break;

                default:
                    memset(_recvMessage, 0, sizeof(_recvMessage));
                    _readLen = recv(clientFd, _recvMessage, BUFFER_SIZE, 0);
                    if (clientClass->getPasswordFlag() && _readLen == -1) {
                        break;
                    } else if (!_readLen) {
                        deleteClient(clientFd, db);
                        break;
                    }
                    clientClass->addBackReadBuffer(_recvMessage);
                    std::string passStr = clientClass->getNextLineReadBuffer();
                    if (passStr.length() != 0) {
                        IrcCommand command1(&db, clientFd);
                        excuteCommand(command1, clientFd, clientClass);
                    }
                }
            }
            if (FD_ISSET(clientFd, &_cpyWrites)) {
                writeUserBuffer(clientFd, clientClass);
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
