#ifndef __IrcCLIENT__
#define __IrcCLIENT__

#include "IRC.hpp"

class IrcServ;
class IrcChannel;

#define MAX_CLIENT_BUFFER_SIZE 1024

class IrcClient {
    public:
        IrcClient(const IrcClient& copy);
        const IrcClient& operator=(const IrcClient& copy);
        IrcClient(int commFd, uint32_t ipAddr, uint16_t portNo);
        ~IrcClient();
        void Display();

    private:

        IrcClient();
        int      _commFd;
        uint32_t _ipAddr;
        uint16_t _portNo;
        // uint32_t _serverIpAddr;
        // uint16_t _serverPortNo;
        
        std::map<std::string, IrcChannel> _registredChannels;
        std::map<std::string, IrcChannel> _operatorChannels;
} ;


#endif