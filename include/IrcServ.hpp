/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:45:49 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/21 18:52:47 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IrcServ_hpp__
#define __IrcServ_hpp__

#define BUFFER_SIZE 512

#include <string>
#include <arpa/inet.h>

#include "../include/IrcClient.hpp"
#include "../include/IrcChannel.hpp"
#include "../include/IrcDB.hpp"
#include "../include/IrcCommand.hpp"

class IrcServ 
{
public:
    IrcServ();
    virtual ~IrcServ();
    IrcServ(int port, std::string passWord);

    int on();
    void run();

private:
    int _error;

    int _port;
    std::string _passWord;

    int _servFd;
    int _fdMax;
    int _fdNum;
    int _opt;

    ssize_t _readLen;
    ssize_t _writeLen;

    fd_set _activeReads;
    fd_set _activeWrites;
    fd_set _cpyReads;
    fd_set _cpyWrites;

    char _recvMessage[BUFFER_SIZE];
    std::string _sendMessage;
    sockaddr_in _servAddr;
    struct timeval _timeout;

    bool initSelect();
    bool acceptClient(int acceptFd, struct sockaddr_in& clientAddr, socklen_t& clientAddrLen, IrcDB& db);
    void deleteClient(int fd, IrcDB& db);
    void displayServerParam(const IrcDB& db);
    bool isSameNickname(IrcDB& db, std::string message);
    void sendTo(int clientFd, std::string message);

    void excuteCommand(IrcCommand& command, const int clientFd, int messageLen, IrcClient* clientClass);
    void writeUserBuffer(const int clientFd, IrcClient* clientClass);

    enum e_active
    {
        EXIT_CLIENT = 0,
        ENTER_CLIENT = 1,
        EMPTY = 0,
    };
    

    IrcServ(const IrcServ &copy);
    const IrcServ &operator=(const IrcServ &copy);

    class socketException : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class bindException : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class listenException : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class selectException : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class acceptException : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class fcntlException : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class setsockoptException : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
};

#endif
