#ifndef __IrcCLIENT__
#define __IrcCLIENT__

#include "IRC.hpp"

class IrcServ;
class IrcChannel;

#define MAX_CLIENT_BUFFER_SIZE 1024

class IrcClient {
    public:
        IrcClient();
        IrcClient(uint32_t ipAddr, uint16_t portNo);
        void Display();
        int     getCommFd();
        unsigned char recv_buffer[MAX_CLIENT_BUFFER_SIZE];
        uint32_t _ipAddr;
        uint16_t _portNo;
        uint32_t _serverIpAddr;
        uint16_t _serverPortNo;
        int      _commFd;        
    private:

        
        std::vector<IrcChannel> _registredChannels;
        std::vector<IrcChannel> _operatorChannels;        
} ;


#endif