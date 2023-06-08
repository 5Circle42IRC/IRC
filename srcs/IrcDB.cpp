#include "../include/IrcDB.hpp"

const char* DB::notFindClient::what() const throw()
{
    return "not find Client";
}

const char* DB::notFindClient::what() const throw()
{
    return "not find channel";
}

DB::DB() { }

DB::~DB() { };

const IrcClient& DB::find(const int clientFd) 
{
    std::map<int, IrcClient&>::iterator client = _client.find(clientFd);

    if (client == _client.end())
        throw notFindClient();
    return client->second;
}

const IrcChannel& DB::find(const std::string channelName) 
{
    std::map<std::string, IrcChannel&>::iterator channel = _channel.find(channelName);

    if (channel == _channel.end())
        throw notFindChannel();
    return channel->second;
}

void DB::deleteChannel(const std::string channelName)
{
    _channel.erase(channelName);
}

void DB::deleteTargetInChannel(const std::string& channel, const int targetFd)
{
    std::map<std::string, IrcChannel&>::iterator temp = _channel.find(channel);

    if (temp == _channel.end())
        throw notFindChannel();
    temp->second.deleteUser(targetFd);
}

void DB::addTargetInChannel(const std::string& channel, const int targetFd)
{
    std::map<std::string, IrcChannel&>::iterator temp = _channel.find(channel);

    if (temp == _channel.end())
        throw notFindChannel();
    temp->second.addUser(targetFd);
}

/**
 * 기본 client만 만들어주기
*/
void DB::registerClient(const int& fd)
{
    _client[fd] = IrcClient(fd, "plz make nickname", "plz make passwd", "");
}

/**
 * 기본 channel만 만들어주기
*/
void DB::registerChannel(const std::string& channel)
{
    _channel[channel] = IrcChannel(channel);// 안되면 깊은 복사 필요.
}