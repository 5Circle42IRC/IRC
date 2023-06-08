#include "../include/IrcDB.hpp"

DB::DB() { }
DB::~DB()
{
    for()
};
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

void DB::deleteTargetInChannel(const std::string& channel, const int targetFd)
{
    IChannel temp = _channel.find(channel);

    if (temp != _channel.end())
        temp._user.erase(targetFd);
}

void DB::addTargetInChannel(const std::string& channel, const int targetFd)
{
    IChannel temp = _channel.find(channel);

    if (temp != _channel.end())
        temp.addUser(clientFd);
}

void DB::registerClient(const int& fd)
{
    _client[fd] = new IClient();
}

void DB::registerChannel(const std::string& channel)
{
    _channel[fd] = new IChannel();
}