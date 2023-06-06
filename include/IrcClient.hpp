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
        
        char recv_buffer[MAX_CLIENT_BUFFER_SIZE];
        uint32_t _ipAddr;
        uint16_t _portNo;
        uint32_t _serverIpAddr;
        uint16_t _serverPortNo;
        int      _commFd;   

        void Display();
        int     getCommFd();

        void setNick(std::string nick);
        std::string getNick();
        bool getNickFlag();

        void setUsername(std::string username);
        std::string getUsername();
        void setHostname(std::string username);
        std::string getHostname();
        void setServername(std::string username);
        std::string getServername();
        void setRealname(std::string username);
        std::string getRealname();
        void setUserFlag();
        bool getUserFlag();
        
        bool checkPASS(std::string pass, std::string serverpass);
        void setPassFlag();
        bool getPassFlag();

        void setRegistFlag();
        bool getRegistFlag();

        bool checkPart(std::string channelname);
        void doPart(std::string channelname);
        std::vector<IrcChannel> _registredChannels;

        std::string nick;
    private:

        
        std::string username;
        std::string hostname;
        std::string servername;
        std::string realname;
        bool        userFlag;
        bool        nickFlag;
        bool        passFlag;
        bool        registFlag;

        std::vector<IrcChannel> _operatorChannels;        
} ;


#endif