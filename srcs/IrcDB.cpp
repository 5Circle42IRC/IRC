#include "../include/IrcDB.hpp"
#include "../include/IrcClient.hpp"
#include "../include/IrcChannel.hpp"
#include "../include/IrcCommand.hpp"
const std::map<std::string, IrcChannel*> IrcDB::getAllChannels() const
{
	return _channels;
}

const std::map<int, IrcClient*> IrcDB::getAllClients() const
{
	return _clients;
}

IrcDB::IrcDB(){}
IrcDB::~IrcDB(){}

IrcClient* IrcDB::findClientByName(std::string name){
	std::map<int, IrcClient *>::iterator it;
	for (it = _clients.begin();
		it != _clients.end();
		it++)
		{
			if (it->second->getNickname() == name)
				return it->second; 
		}
	throw ERR_NOSUCHNICK();
}
IrcClient* IrcDB::findClientByFd(int clientFd){
	std::map<int, IrcClient *>::iterator it = _clients.find(clientFd);
	if (it == _clients.end())
		throw ERR_CLIENT_NOT_IN_DB();
	return it->second;
}

IrcChannel* IrcDB::findChannel(std::string name){
	std::map<std::string, IrcChannel *>::iterator it = _channels.find(name);
	if (it == _channels.end())
		throw ERR_CHANNEL_NOT_IN_DB();
	return it->second;
}

//ㄷㅡㄹ어 오오는  애애가가
// Client A   Fd 4
// old    Client B Fd 4
void IrcDB::insertClient(IrcClient *client){
	_clients[client->getFd()] = client;
}
void IrcDB::insertChannel(IrcChannel *channel){
	//std::string teststring("tttt");
	std::string teststring = channel->getName();
	_channels.insert(std::make_pair(teststring, channel));

}

void IrcDB::deleteClient(int clientFd){
	delete _clients[clientFd];
	_clients.erase(clientFd);
}
void IrcDB::deleteChannel(std::string name){
	delete _channels[name];
	_channels.erase(name);
}

const char* IrcDB::ERR_CHANNEL_NOT_IN_DB::what() const throw() { return "ERR_CHANNEL_NOT_IN_DB"; }
const char* IrcDB::ERR_CLIENT_NOT_IN_DB::what() const throw() { return "ERR_CLIENT_NOT_IN_DB"; }
const char* IrcDB::ERR_NOSUCHNICK::what() const throw() { return "ERR_NOSUCHNICK"; }