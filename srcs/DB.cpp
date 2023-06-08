#include "../include/IrcDB.hpp"

DB::DB() { }
DB::~DB(){};
const IClient& DB::find(const int clientFd) const
{
    const IClient& client = _client.find(clientFd) 

    if (client == _client.end())
        throw notFindClient;
    return client;
}

const IChannel& DB::find(const std::string channelName) const
{
    const IChannel& channel = _channel.find(channelName);

    if (channel == _channel.end())
        throw notFindChannel;
    return channel;
}

void DB::deleteChannel(const std::string channelName)
{
    _channel.erase(channelName);
}

virtual void deleteTargetInChannel(const std::string& channel, const int targetFd)
{
    
}

virtual void addTargetInChannel(const std::string& Channel, const int targetFd)
{

}

virtual void registerClient(const int& fd)
{

}