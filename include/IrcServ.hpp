#ifndef __IrcServ_hpp__
#define __IrcServ_hpp__

#include "IRC.hpp"

class IrcClient;
class IrcChannel;

class IrcServ
{
public:
    IrcServ(int port, std::string passWord);
    void run();
    ~IrcServ();

private:
    int         _port;
    std::string _passWord;

    int         _servFd;
    int         _fdMax;
    int         _fdNum;

    bool        _opt;

    fd_set      _activeReads;
    fd_set      _activeWrites;
    fd_set      _cpyReads;
    fd_set      _cpyWrites;

    struct sockaddr_in                  _servAddr;
    socklen_t                           _bsize;
    std::map<int, IrcClient>            _clients;
    std::map<std::string, IrcChannel>   _channels;

    bool                                _isError;
    
    IrcServ();
    IrcServ(const IrcServ& copy);
    const IrcServ& operator=(const IrcServ& copy);
};

#endif