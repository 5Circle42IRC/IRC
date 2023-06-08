#ifndef __IRC_DB_HPP__
#define __IRC_DB_HPP__

#include "../InterfaceClass/IDB.hpp"
#include "../include/IrcChannel.hpp"
#include "../include/IrcClient.hpp"
#include <exception>

class DB : private IrcClient, private IrcChannel
{
public:
    DB();
    ~DB();
	virtual const IrcClient& find(const int clientFd);
	virtual const IrcChannel& find(const std::string channelName);
	virtual void deleteChannel(const std::string channelName);
	virtual void deleteTargetInChannel(const std::string& channelName, const int targetFd);
	virtual void addTargetInChannel(const std::string& channelName, const int targetFd);
	virtual void registerClient(const int& fd);
	virtual void registerChannel(const std::string& channel);

private:
	class notFindClient : public std::exception
	{
		virtual const char* what() const throw();
	};

	class notFindChannel : public std::exception
	{
		virtual const char* what() const throw();
	};

	std::map<int, IrcClient &> _client;
	std::map<std::string, IrcChannel &> _channel;
    const DB& operator=(const DB& copy);
	DB(const DB& copy);

};

#endif