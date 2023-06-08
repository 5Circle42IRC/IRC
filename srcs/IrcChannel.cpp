#include <arpa/inet.h>
#include "../include/Irc.hpp"

IrcChannel::IrcChannel() 
    : _channelName("no Name"), _channelTopic("any thing")
{ }

IrcChannel::IrcChannel(std::string channelName)
    : _channelName(channelName), _channelTopic("any thing")
{ }

IrcChannel::~IrcChannel() {};

IrcChannel::IrcChannel(const IrcChannel& copy)
    : _channelName(copy._channelName), _channelTopic(copy._channelTopic)
{
    if (this != &copy)
        throw CopyError();
}

const IrcChannel& IrcChannel::operator=(const IrcChannel& copy)
{
    if (this != &copy)
    {
        _channelName = copy._channelName;
        _channelTopic = copy._channelTopic;
    }
    return *this;
}
