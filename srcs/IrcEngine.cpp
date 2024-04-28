/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcEngine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:46:28 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/26 13:16:01 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/IrcEngine.hpp"
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>

IrcEngine::IrcEngine(){};
IrcEngine::~IrcEngine(){};
IrcEngine::IrcEngine(int port)
    : _error(0), _port(port), _servFd(0), _fdMax(3), _fdNum(0), _opt(1), _readLen(0), _writeLen(0)
{
}

int IrcEngine::on()
{
    std::memset(_recvMessage, 0, sizeof(_recvMessage));
    std::memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _servAddr.sin_port = htons(_port);
    _servFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_servFd == -1)
        throw IrcEngine::socketException();

    _error = fcntl(_servFd, F_SETFL, O_NONBLOCK);
    if (_error == -1)
        throw IrcEngine::fcntlException();

    _error = setsockopt(_servFd, SOL_SOCKET, SO_REUSEADDR, &_opt, static_cast<socklen_t>(sizeof(_opt)));
    if (_error)
        throw IrcEngine::setsockoptException();

    _error = bind(_servFd, (struct sockaddr *)&_servAddr, sizeof(_servAddr));
    if (_error)
        throw IrcEngine::bindException();

    _error = listen(_servFd, 100);
    if (_error)
        throw IrcEngine::listenException();
    FD_ZERO(&_activeReads);
    FD_ZERO(&_activeWrites);
    FD_SET(_servFd, &_activeReads);
    _fdMax = _servFd;
    return _servFd;
}

int IrcEngine::initSelect()
{
    FD_COPY(&_activeReads, &_cpyReads);
    FD_COPY(&_activeWrites, &_cpyWrites);
    _timeout.tv_sec = 0;
    _timeout.tv_usec = 0;

    return select(_fdMax + 1, &_cpyReads, &_cpyWrites, 0, &_timeout);
}

bool IrcEngine::acceptClient(int acceptFd, struct sockaddr_in &clientAddr, socklen_t &clientAddrLen, IrcDB &db)
{
    std::memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddrLen = sizeof(clientAddr);
    acceptFd = accept(_servFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (acceptFd == -1 || fcntl(acceptFd, F_SETFL, O_NONBLOCK) == -1)
        return false;

    db.insertClient(new IrcClient(acceptFd, "", "", "", &_activeWrites));
    db.findClientByFd(acceptFd)->addBackCarriageBuffer("input password");
    FD_SET(acceptFd, &_activeReads);
    FD_SET(acceptFd, &_activeWrites);
    if (_fdMax < acceptFd)
        _fdMax = acceptFd;
    return true;
}

void IrcEngine::deleteClient(int clientFd, IrcDB &db)
{
    std::map<std::string, IrcChannel *> _channels = db.getAllChannels();
    std::map<std::string, IrcChannel *>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); it++)
    {
        if (it->second->isJoinedUser(clientFd))
            it->second->deleteUser(clientFd);
        if (it->second->getUser().size() == 0)
            db.deleteChannel(it->second->getName());
    }
    db.deleteClient(clientFd);
    FD_CLR(clientFd, &_activeReads);
    FD_CLR(clientFd, &_activeWrites);
    close(clientFd);
}

bool IrcEngine::isSameNickname(IrcDB &db, std::string message)
{
    std::map<int, IrcClient *> clients(db.getAllClients());

    for (std::map<int, IrcClient *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (!it->second->getNickname().compare(message))
            return true;
    }
    return false;
}

void IrcEngine::excuteCommand(IrcCommand &command, const int clientFd, IrcClient *clientClass)
{
    command.setClientFd(clientFd).parsing(clientClass->getNextLineReadBuffer());
    clientClass->reduceReadBuffer(clientClass->getNextLineReadBuffer().size() + 1);
}

void IrcEngine::writeUserBuffer(const int clientFd, IrcClient *clientClass)
{
    if (clientClass->getBuffer().size())
    {
        _sendMessage = clientClass->getBuffer();
        _writeLen = send(
            clientFd,
            _sendMessage.c_str(),
            _sendMessage.size(),
            0);
        if (_writeLen == -1)
            return;
        if (_writeLen == static_cast<ssize_t>(_sendMessage.size()))
            FD_CLR(clientFd, &_activeWrites);
        clientClass->reduceBuffer(_writeLen);
    }
}

void IrcEngine::run()
{
    int acceptFd(0);
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    IrcClient *clientClass;
    int selectResult = 0;

    while (42)
    {
        selectResult = initSelect();
        for (int clientFd = 0; 0 < selectResult; clientFd++)
        {
            // TODO : 여기에서 바뀌어야함.
            try
            {
                if (clientClass->getBuffer().length() != EMPTY)
                {
                    FD_SET(clientFd, &_activeWrites);
                }
            }
            catch (std::exception &e)
            {
            }
            catch (...)
            {
            }

            if (FD_ISSET(clientFd, &_cpyReads))
            {
                switch (static_cast<int>(clientFd == _servFd))
                {
                case ENTER_CLIENT:
                    if (!acceptClient(acceptFd, clientAddr, clientAddrLen, db))
                    {
                    }
                    break;

                default:
                    memset(_recvMessage, 0, sizeof(_recvMessage));
                    _readLen = recv(clientFd, _recvMessage, BUFFER_SIZE, 0);
                    if (_readLen == -1)
                    {
                        break;
                    }
                    else if (!_readLen)
                    {
                        deleteClient(clientFd, db);
                        break;
                    }
                    clientClass->addBackReadBuffer(_recvMessage);
                    std::string passStr = clientClass->getNextLineReadBuffer();
                    if (passStr.length() != 0)
                    {
                        IrcCommand command1(&db, clientFd);
                        excuteCommand(command1, clientFd, clientClass);
                    }
                }
                selectResult--;
            }
            if (FD_ISSET(clientFd, &_cpyWrites))
            {
                writeUserBuffer(clientFd, clientClass);
                selectResult--;
            }
        }
    }
}

const char *IrcEngine::socketException::what() const throw()
{
    return "socket error";
}

const char *IrcEngine::fcntlException::what() const throw()
{
    return "fcntl error";
}

const char *IrcEngine::setsockoptException::what() const throw()
{
    return "setsockopt error";
}

const char *IrcEngine::bindException::what() const throw()
{
    return "bind error";
}

const char *IrcEngine::listenException::what() const throw()
{
    return "listen error";
}

const char *IrcEngine::selectException::what() const throw()
{
    return "select error";
}

const char *IrcEngine::acceptException::what() const throw()
{
    return "accept error";
}
