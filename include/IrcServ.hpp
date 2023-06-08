#ifndef __IrcServ_hpp__
#define __IrcServ_hpp__

#include "Irc.hpp"

class IrcClient;
// class IrcChannel;

class IrcServ
{
public:
    ~IrcServ();
    IrcServ(int port, std::string passWord);

    void run();
    int GetMaxFd();
    void CopyClientFDtoFDSet(fd_set *fdset);
    void RemoveClients(IrcClient *ircClient);
    void AddClients(IrcClient *ircClient);
private:
    bool        _isError;

    int         _port;
    std::string _passWord;

    int         _servFd;
    int         _fdMax;
    int         _fdNum;
    int         _opt;

    fd_set      _activeReads;
    fd_set      _activeWrites;
    fd_set      _cpyReads;
    fd_set      _cpyWrites;

    sockaddr_in                         _servAddr;
    socklen_t                           _servAddrLen;
    std::map<int, IrcClient>            _clients;
    std::map<std::string, IrcChannel>   _channels;


    IrcServ();
    IrcServ(const IrcServ& copy);
    const IrcServ& operator=(const IrcServ& copy);
};

#endif