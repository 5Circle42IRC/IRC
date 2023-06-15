#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::joinChannel(std::map<std::string, std::string>& keypair){
	IrcChannel *channel;
	IrcClient *client = _db->findClientByFd(_clientFd);
	std::string name = keypair.begin()->first;
	std::string key = keypair.begin()->second;
	try {
		channel =_db->findChannel(name);
	} catch(std::exception &e){
		channel = new IrcChannel(name);
		channel->addUser(_clientFd);
		channel->setOperator(_clientFd, _clientFd);
		if (key.size() != 0){
			channel->setPassword(key);
			channel->setGrant(M_KEY, true);
		}
		_db->insertChannel(channel);
		client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName());
		return ;
	}
	if (channel->isJoinedUser(_clientFd))
			throw ERR_USER_ON_CHANNEL();
	if ((channel->getGrant() & M_KEY) && channel->getPassword().compare(key))
			throw ERR_INVALID_PASSWORD();
	channel->addUser(_clientFd);
	client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName() + "\n");
}


void IrcCommand::JOIN(){
	std::map<int, std::map<std::string, std::string> > argsList;
	std::map<std::string, std::string>keypair;

	if (_args.begin()->at(0) != '#')
		throw ERR_INVALID_ARGUMENT();
	int i, j = 0;
	std::deque<std::string>::iterator it;
	for (it = _args.begin(); it != _args.end() && it->at(0) == '#'; it++){
		keypair[*it] = "";
		argsList[i] = keypair;
		i++;
	}
	for (; it != _args.end() && it->at(0) != '#'; it++){
		argsList[j].begin()->second = *it;
		j++;
	}
	if (it != _args.end())
		throw ERR_INVALID_ARGUMENT();
	for (std::map<int, std::map<std::string, std::string> >::iterator it2 = argsList.begin(); it2 != argsList.end(); it2++){
		joinChannel(it2->second);
	}
}