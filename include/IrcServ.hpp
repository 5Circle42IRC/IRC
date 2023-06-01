#ifndef __IrcServ_hpp__
#define __IrcServ_hpp__

#include "IRC.hpp"

class IrcClient;
class IrcChannel;

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
    int         _servFd;
    int         _port;
    int         _fdMax;
    int         _fdNum;

    fd_set      _activeReads;
    fd_set      _activeWrites;
    fd_set      _cpyReads;
    fd_set      _cpyWrites;

    struct sockaddr_in          _servAddr;
    std::string                 _passWord;
    std::map<int, IrcClient>     _clients;//참조를 넣어서 쓰는 법 모르겠음
                                        //vector는 배열이라서 삭제가 불편함
                                        //list는 remove로 바로 삭제 가능
    std::map<std::string, IrcChannel>     _channels;//참조를 넣어서 쓰는 법 모르겠음
    
    IrcServ();
    IrcServ(const IrcServ& copy);
    const IrcServ& operator=(const IrcServ& copy);
};

#endif