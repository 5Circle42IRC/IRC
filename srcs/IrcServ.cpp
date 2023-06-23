/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:46:28 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/23 20:10:54 by jwee             ###   ########.fr       */
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
    if (acceptFd == -1 || fcntl(acceptFd, F_SETFL, O_NONBLOCK) == -1)
        return false;
    sendTo(acceptFd, "input server password");
    db.insertClient(new IrcClient(acceptFd, "", "", ""));
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

void IrcServ::displayServerParam(const IrcDB& db)
{
	std::map<std::string , IrcChannel*> channels(db.getAllChannels());
	std::map<int, IrcClient *> clients(db.getAllClients());
    
    std::cout << "\033[31m" << "--------------------------------------" << std::endl;
    for(std::map<int, IrcClient *>::iterator clientIt = clients.begin(); clientIt != clients.end(); clientIt++)
    {
        std::cerr << "client fd       : " << clientIt->first << std::endl;
        std::cerr << "client username : " << "[" << clientIt->second->getUsername() << "]" << std::endl;
        std::cerr << "client nickname : " << "[" << clientIt->second->getNickname() << "]" << std::endl;
        std::cerr << "client password : " << "[" << clientIt->second->getPassword() << "]" << std::endl;
    }

    std::cout << "======================================" << std::endl;
    std::cerr << "channl all size : " << "[" << channels.size() << "]" << std::endl;
    for (std::map<std::string, IrcChannel*>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::cout << "=-=-=-=-=-=-=-=-=-=" << "[ "  
                  << "channel name : " << it->second->getName() << " ]" 
                  << "-=-=-=-=-=-=-=-=-=-" << std::endl;
        std::cerr << "channel password : " << "[" << it->second->getPassword() << "]" << std::endl;
        std::cerr << "channel limit : " << "[" << it->second->getLimit() << "]" << std::endl;
        std::cerr << "channel grant : " << "[" << it->second->getGrant() << "]" << std::endl;
        std::cerr << "channel topic : " << "[" << it->second->getTopic() << "]" << std::endl;
    }

    std::cout << "======================================" << std::endl;
    std::cerr << "server recieve Message : " << "[" << _recvMessage << "]" << std::endl;
    std::cerr << "server recieve Message len : " << std::strlen(_recvMessage) << std::endl;
    std::cout << "--------------------------------------" << "\033[0m" << std::endl;
}

void IrcServ::sendTo(int clientFd, std::string message)
{
    std::string sendMessage("\033[38;5;3m" + message + "\r\n" + "\033[0m");
    send(clientFd, sendMessage.c_str(), sendMessage.length(), 0);
}

void IrcServ::checkServerPassword(const int clientFd, IrcClient* clientClass, IrcDB& db)
{
    if (!_passWord.compare(_recvMessage))
    {
        clientClass->setPasswordFlag(true);
        sendTo(clientFd, "input nickname");
    } else {
        sendTo(clientFd, "Failed Password, plz connecting again");

        deleteClient(clientFd, db);
    }
}

void IrcServ::checkNickname(const int clientFd, const int messageLen, IrcDB& db, IrcClient* clientClass)
{
    if (messageLen < 2|| messageLen > 10) {
        sendTo(clientFd, "wrong input retry");
    } else {
        sendTo(clientFd, "set your password");
        _recvMessage[messageLen - 1] = '\0';
        if (!isSameNickname(db, _recvMessage)) {
            clientClass->setNickname(_recvMessage);
        } else {
            sendTo(clientFd, "The name already exists");
        }
    }
}

void IrcServ::checkUserPassword(const int messageLen, const int clientFd, IrcClient* clientClass)
{
    if (messageLen < 2) {
        sendTo(clientFd, "wrong input retry");
    } else {
        _recvMessage[messageLen - 1] = '\0';
        clientClass->setPassword(_recvMessage);
        sendTo(clientFd, "input realname");
    }
}

void IrcServ::checkUserName(const int clientFd, const int messageLen, IrcClient* clientClass)
{
    if (messageLen < 2) {
        sendTo(clientFd, "wrong input retry");
    } else {
        _recvMessage[messageLen - 1] = '\0';
        clientClass->setUsername(_recvMessage);
    }
}

void IrcServ::excuteCommand(IrcCommand& command, const int clientFd, int messageLen, IrcClient* clientClass)
{
    try {
        if (messageLen > 1)
        {
            command.setClientFd(clientFd).parsing(clientClass->getNextLineReadBuffer());
            clientClass->reduceReadBuffer(clientClass->getNextLineReadBuffer().size() + 1);
        }
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
        std::cerr << "------ user "<< clientClass->getNickname() 
                  << " recieve massage ------\n" 
                  << clientClass->getBuffer()
                  << "---------- recieve massage -----------\n" << "" << std::endl;
        }
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

    std::cout << "*" << _passWord << "*" << std::endl;
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
                        std::cerr << "failed accept" << std::endl;
                    break;

                default:
                    memset(_recvMessage, 0, sizeof(_recvMessage));
                    _readLen = recv(clientFd, _recvMessage, BUFFER_SIZE, 0);
                    if (clientClass->getPasswordFlag() && _readLen == -1) {
                        std::cerr << "failed recv" << std::endl;
                        break;
                    } else if (!_readLen) {
                        std::cerr << "exit_Client" << std::endl;
                        deleteClient(clientFd, db);
                        break;
                    }
                    clientClass->addBackReadBuffer(_recvMessage);
                    std::string passStr = clientClass->getNextLineReadBuffer();
                    if (passStr.length() != 0) {
                        IrcCommand command1(&db, clientFd);
                        excuteCommand(command1, clientFd, passStr.length(), clientClass);
                        displayServerParam(db);
                    }
                }
            }
            if (FD_ISSET(clientFd, &_cpyWrites)) {
                writeUserBuffer(clientFd, clientClass);
                FD_CLR(clientFd, &_activeWrites);
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
