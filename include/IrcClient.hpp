#ifndef __IrcCLIENT__
#define __IrcCLIENT__

#include "Irc.hpp"

class IrcServ;
// class IrcChannel;

#define MAX_CLIENT_BUFFER_SIZE 1024

class IrcClient {
    public:
        IrcClient();
        IrcClient(const IrcClient& copy);
        const IrcClient& operator=(const IrcClient& copy);
        IrcClient(int commFd, sockaddr_in _addr, socklen_t _ipAddrLen);
        ~IrcClient();
        void Display();

    private:

        int         _commFd;
        sockaddr_in _addr;
        socklen_t   _addrLen;
        // uint32_t _serverIpAddr;
        // uint16_t _serverPortNo;

        
        // std::map<std::string, IrcChannel> _registredChannels;
        // std::map<std::string, IrcChannel> _operatorChannels;
} ;


#endif