#ifndef __IRC_DB_HPP__
#define __IRC_DB_HPP__

#include "../InterfaceClass/IDB.hpp"
#include "../include/IrcChannel.hpp"
#include "../include/IrcClient.hpp"
#include <exception>

class IrcDB : protected IrcClient, protected IrcChannel
{
public:
    IrcDB();
    virtual ~IrcDB();
	virtual const IrcClient& find(const int clientFd);
	virtual const IrcChannel& find(const std::string channelName);
	virtual void deleteChannel(const std::string channelName);
	virtual void deleteTargetInChannel(const std::string& channelName, const int targetFd);
	virtual void addTargetInChannel(const std::string& channelName, const int targetFd);
	virtual void registerClient(const int& fd);
	virtual void registerChannel(const std::string& channel);

protected:
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

private:
    const IrcDB& operator=(const IrcDB& copy);
	IrcDB(const IrcDB& copy);

};

#endif