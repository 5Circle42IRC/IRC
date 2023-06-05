#include <arpa/inet.h>
#include "../include/IRC.hpp"

IrcChannel::IrcChannel(std::string channelName) {

    this->_channelName = channelName;
}


std::string IrcChannel::getChannelName()
{
    return this->_channelName;
}

void
IrcChannel::Display() {

}
