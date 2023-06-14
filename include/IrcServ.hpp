#ifndef __IrcServ_hpp__
#define __IrcServ_hpp__

#define BUFFER_SIZE 512

#include <string>
#include <arpa/inet.h>

#include "../include/IrcClient.hpp"
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

    char _message[BUFFER_SIZE];
    sockaddr_in _servAddr;
    struct timeval _timeout;

    bool initSelect();
    bool acceptClient(int acceptFd, struct sockaddr_in& clientAddr, socklen_t& clientAddrLen);
    void deleteClient(int fd);


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