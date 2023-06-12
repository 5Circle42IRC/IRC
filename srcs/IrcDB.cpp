#include "../include/IrcDB.hpp"
#include "../include/IrcClient.hpp"
#include "../include/IrcChannel.hpp"

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
	return NULL;
}
IrcClient* IrcDB::findClientByFd(int clientFd){
	std::map<int, IrcClient *>::iterator it = _clients.find(clientFd);
	if (it == _clients.end())
		throw std::exception();
	return it->second;
}

IrcChannel* IrcDB::findChannel(std::string name){
	std::map<std::string &, IrcChannel *>::iterator it = _channels.find(name);
	if (it == _channels.end())
		throw std::exception();
	return it->second;
}

//ㄷㅡㄹ어 오오는  애애가가
// Client A   Fd 4
// old    Client B Fd 4
void IrcDB::insertClient(IrcClient *client){
	_clients[client->getFd()] = client;
}
void IrcDB::insertChannel(IrcChannel *channel){
	_channels[channel->getName()] = channel;
}

void IrcDB::deleteClient(int clientFd){
	delete _clients[clientFd];
	_clients.erase(clientFd);
}
void IrcDB::deleteChannel(std::string name){
	delete _channels[name];
	_channels.erase(name);
}