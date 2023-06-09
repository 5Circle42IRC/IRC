#include "../include/IrcDB.hpp"

const char* IrcDB::notFindClient::what() const throw()
{
    return "not find Client";
}

const char* IrcDB::notFindChannel::what() const throw()
{
    return "not find channel";
}

IrcDB::IrcDB() { }

IrcDB::~IrcDB() { };

IrcClient& IrcDB::find(const int clientFd) 
{
    std::map<int, IrcClient&>::iterator client = _client.find(clientFd);

    if (client == _client.end())
        throw notFindClient();
    return client->second;
}

IrcChannel& IrcDB::find(const std::string channelName) 
{
    std::map<std::string, IrcChannel&>::iterator channel = _channel.find(channelName);

    if (channel == _channel.end())
        throw notFindChannel();
    return channel->second;
}

void IrcDB::deleteChannel(const std::string channelName)
{
    _channel.erase(channelName);
}

void IrcDB::deleteTargetInChannel(const std::string& channel, const int targetFd)
{
    std::map<std::string, IrcChannel&>::iterator temp = _channel.find(channel);

    if (temp == _channel.end())
        throw notFindChannel();
    temp->second.deleteUser(targetFd);
}

void IrcDB::addTargetInChannel(const std::string& channel, const int targetFd)
{
    std::map<std::string, IrcChannel&>::iterator temp = _channel.find(channel);

    if (temp == _channel.end())
        throw notFindChannel();
    temp->second.addUser(targetFd);
}

/**
 * 기본 client만 만들어주기
*/
void IrcDB::registerClient(const int& fd)
{
    _client[fd] = IrcClient(fd, "plz make nickname", "plz make passwd", "");
}

/**
 * 기본 channel만 만들어주기
*/
void IrcDB::registerChannel(const std::string& channel)
{
    _channel[channel] = IrcChannel(channel);// 안되면 깊은 복사 필요.
}