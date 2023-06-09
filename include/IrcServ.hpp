#ifndef __IrcServ_hpp__
#define __IrcServ_hpp__

#define BUFFER_SIZE 1024

#include "../include/IrcCommand.hpp"

class IrcServ : protected IrcCommand
{
public:
    IrcServ();
    virtual ~IrcServ();
    IrcServ(int port, std::string passWord);

    void run();

protected:
    bool _isError;

    int _port;
    std::string _passWord;

    int _servFd;
    int _fdMax;
    int _fdNum;
    int _opt;

    fd_set _activeReads;
    fd_set _activeWrites;
    fd_set _cpyReads;
    fd_set _cpyWrites;

    sockaddr_in _servAddr;

private:
    bool addClient(const int i);
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