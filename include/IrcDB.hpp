#ifndef __IRC_DB_HPP__
#define __IRC_DB_HPP__

#include <iostream>
#include <map>
#include <utility>

class IrcClient;
class IrcChannel;

class IrcDB
{
public:
    IrcDB();
    ~IrcDB();

	IrcClient *findClientByName(std::string name);
	IrcClient *findClientByFd(int clientFd);

	IrcChannel *findChannel(std::string name);

	void insertClient(IrcClient *client);
	void insertChannel(IrcChannel *channel);

	void deleteClient(int clientFd);
	void deleteChannel(std::string name);
private:
	std::map<std::string& , IrcChannel*> _channels;
	std::map<int, IrcClient *> _clients;
};

#endif