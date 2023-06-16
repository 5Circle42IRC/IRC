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

	const std::map<std::string, IrcChannel*> getAllChannels() const;
	const std::map<int, IrcClient*> getAllClients() const;

	class ERR_CHANNEL_NOT_IN_DB: public std::exception{
		virtual const char *what() const throw();
	};
	class ERR_CLIENT_NOT_IN_DB: public std::exception{
		virtual const char *what() const throw();
	};
	class ERR_NOSUCHNICK : public std::exception {
		virtual const char *what() const throw();
	};		

private:
	std::map<std::string , IrcChannel*> _channels;
	std::map<int, IrcClient *> _clients;
};

#endif