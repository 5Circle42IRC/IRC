#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"
#include <algorithm>

void IrcCommand::joinChannel(std::string name, std::string key){
	IrcChannel *channel;
	IrcClient *client = _db->findClientByFd(_clientFd);
	try {
		channel =_db->findChannel(name);
		if (name.size() > 200)
			throw ERR_INVALID_NAME_OF_CHANNEL();
		if (name.find_first_of(",") >= 0 || name.find_first_of(",") >= 0)
			throw ERR_INVALID_CHAR_IN_NAME();
		if (channel->isJoinedUser(_clientFd))
				throw ERR_USER_ON_CHANNEL();
		if ((channel->getGrant() & M_KEY) && channel->getPassword().compare(key))
				throw ERR_INVALID_PASSWORD();
		channel->addUser(_clientFd);
		channel->setOperator(_clientFd, _clientFd);
		if (channel->getGrant() & M_KEY)
			client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName()+ " using key " + key + "\r\n");
		else
			client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName() + "\r\n");
	} catch(IrcDB::ERR_CHANNEL_NOT_IN_DB &e){
		channel = new IrcChannel(name);
		channel->addUser(_clientFd);
		channel->setOperator(_clientFd, _clientFd);
		if (key.size() != 0){
			channel->setPassword(key);
			channel->setGrant(M_KEY, true);
			client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName()+ " using key " + key + "\r\n");
		}
		else
			client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName()+ "\r\n");		
		_db->insertChannel(channel);
	} catch (std::exception &e){
		_db->findClientByFd(_clientFd)->addBackCarriageBuffer(e.what());
	}

}


void IrcCommand::JOIN(){
	std::map<int, std::string>argsList;
	std::multimap<std::string, std::string>keypair;

	if (_args.begin()->at(0) != '#')
		throw ERR_INVALID_ARGUMENT();
	int i = 0;
	int j = 0;
	std::deque<std::string>::iterator it;
	for (it = _args.begin(); it != _args.end() && it->at(0) == '#'; it++){
		argsList[i] = *it;
		keypair.insert(std::make_pair(*it, ""));
		i++;
	}
	for (; it != _args.end() && it->at(0) != '#'; it++){
		keypair.find(argsList[j])->second = *it;
		j++;
		if (j > i - 1)
			throw ERR_INVALID_ARGUMENT();
	}
	for (std::multimap<std::string, std::string>::iterator it3 = keypair.begin(); it3 != keypair.end(); it3++){
		std::cout << "argsList : " << it3->first << " ****" << std::endl;
	}
	if (it != _args.end())
		throw ERR_INVALID_ARGUMENT();
	for (std::map<std::string, std::string>::iterator it2 = keypair.begin(); it2 != keypair.end(); it2++){
			joinChannel(it2->first, it2->second);
	}
}