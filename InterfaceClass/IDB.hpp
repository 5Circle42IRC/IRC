#ifndef __IDB_HPP__
#define __IDB_HPP__

#include "../InterfaceClass/IChannel.hpp"
#include "../InterfaceClass/IClient.hpp"
#include <map>

class IDB : public IClient, public IChannel
{
public:
	virtual const IClient& find(const int clientFd) const = 0;
	virtual const IChannel& find(const std::string& channelName) const = 0;
	virtual void deleteChannel(const std::string& channelName) = 0;
	virtual void deleteTargetInChannel(const std::string& channelName, const int targetFd) = 0;
	virtual void addTargetInChannel(const std::string& channelName, const int targetFd) = 0;
	virtual void registerClient(const int& fd) = 0;
	virtual void registerChannel(const std::string& channel) = 0;

protected:
	std::map<int, IClient &> _client;
	std::map<std::string, IChannel &> _channel;
};

#endif