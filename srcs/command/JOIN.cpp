#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

static void joinChannel(IrcDB *_db, std::map<std::string, std::string>& keypair,int clientFd){
	IrcChannel *channel;
	IrcClient *client = _db->findClientByFd(clientFd);
	std::string name = keypair.begin()->first;
	std::string key = keypair.begin()->second;
	try {
		channel =_db->findChannel(name);
	} catch(std::exception &e){
		channel = new IrcChannel(name);
		channel->addUser(clientFd);
		channel->setGrant(OPERATOR, 1);
		channel->setOperator(clientFd, clientFd);
		if (key.size() != 0)
			channel->setPassword(key);
		client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName());
	}
	if (channel->getPassword == key)
		channel->addUser(clientFd);
	else
		throw wrongPassword();
	client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName());

}


void IrcCommand::JOIN(int clientFd){
	std::map<int, std::map<std::string, std::string>> argsList;
	std::map<std::string, std::string>keypair;

	std::deque<std::string>::iterator it = _args.begin();

	int i, j = 0;
	for (it; it != _args.end(); it++){
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
	std::map<int, std::map<std::string, std::string>>::iterator it2 = argsList.begin();
	for (it2; it2 != argsList.end(); it2++){
		joinChannel(_db, it2->second, clientFd);
	}
}