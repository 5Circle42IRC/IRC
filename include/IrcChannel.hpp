#ifndef __IrcCHANNEL__
#define __IrcCHANNEL__

#include "IRC.hpp"

class IrcServ;
class IrcClient;

#define MAX_CLIENT_BUFFER_SIZE 1024

class IrcChannel {
public:
    IrcChannel(std::string channelName);
    void Display();

private:    
    std::string _channelName;
    std::string _channelTopic;
    std::map<std::string, IrcChannel> _registredChannels;
    std::map<std::string, IrcChannel> _operatorChannels;
} ;


#endif