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
    int GetMaxFd();
    void CopyClientFDtoFDSet(fd_set *fdset);
    void RemoveClients(IrcClient *ircClient);
    void AddClients(IrcClient *ircClient);
    void CommandHandler(IrcClient *ircClient);
    void nick_command(IrcClient *ircClient, std::string argus); 
    void user_command(IrcClient *ircClient, std::string argus); 
    void pass_command(IrcClient *ircClient, std::string argus);
    void part_command(IrcClient *ircClient, std::string argus);
    void priv_command(IrcClient *ircClient, std::string argus);
    std::string getPassword(); 
    std::vector <IrcChannel>    _channels;
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
    std::list <IrcClient*>     _clients;//참조를 넣어서 쓰는 법 모르겠음
                                        //vector는 배열이라서 삭제가 불편함
                                        //list는 remove로 바로 삭제 가능

    
    IrcServ();
    IrcServ(const IrcServ& copy);
    const IrcServ& operator=(const IrcServ& copy);
};

#endif