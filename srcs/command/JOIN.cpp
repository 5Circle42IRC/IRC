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
		channel->setGrant(M_OPERATOR, 1);
		channel->setOperator(_clientFd, _clientFd);
		if (key.size() != 0)
			channel->setPassword(key);
		_db->insertChannel(channel);
		client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName());
		return ;
	}
	if (channel->isJoinedUser(_clientFd))
			throw ERR_USER_ON_CHANNEL();
	if ((channel->getGrant() & M_KEY) && channel->getPassword().compare(key))
			throw ERR_INVALID_PASSWORD();
	channel->addUser(_clientFd);
	client->addBackCarriageBuffer(client->getNickname() + ": JOIN " + channel->getName());
}


void IrcCommand::JOIN(){
	std::map<int, std::map<std::string, std::string> > argsList;
	std::map<std::string, std::string>keypair;

	int i, j = 0;
	for (std::deque<std::string>::iterator it = _args.begin(); it != _args.end(); it++){
		if (it->at(0) == '#'){
			keypair[*it] = "";
			argsList[i] = keypair;
			i++;
		}
		else {
			argsList[j].begin()->second = *it;
			j++;
		}
	}
	for (std::map<int, std::map<std::string, std::string> >::iterator it2 = argsList.begin(); it2 != argsList.end(); it2++){
		joinChannel(it2->second);
	}
}