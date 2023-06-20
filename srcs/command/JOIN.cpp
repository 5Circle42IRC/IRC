#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"
#include <algorithm>
#include <stdio.h>

void IrcCommand::makeBufferString(IrcChannel *channel, IrcClient *client){
	std::string joinedUserList;
	std::map<int, bool> userList = channel->getUser();
	for (std::map<int, bool>::iterator it = userList.begin(); it != userList.end(); it++){
		joinedUserList += (_db->findClientByFd(it->first)->getNickname() + ", ");
	}
	client->addBackCarriageBuffer("<Channel Info>");
	client->addBackCarriageBuffer("< Name       : " + channel->getName() + " >");
	client->addBackCarriageBuffer("< Topic      : " + channel->getTopic() + " >");
	client->addBackCarriageBuffer("< joinedUser : " + joinedUserList + " >");
}

void IrcCommand::joinChannel(std::string name, std::string key){
	IrcChannel *channel;
	IrcClient *client = _db->findClientByFd(_clientFd);
	if (name.at(0) != '#')
		throw ERR_INVALID_NAME_OF_CHANNEL();
	if (name.size() > 200)
		throw ERR_INVALID_NAME_OF_CHANNEL();
	try {
		channel =_db->findChannel(name);
		if (channel->isJoinedUser(_clientFd))
				throw ERR_USERONCHANNEL();
		// grant 검사(mode)
		if ((channel->getGrant() & M_KEY) && channel->getPassword().compare(key))
				throw ERR_BADPASSWORD();
		if ((channel->getGrant() & M_LIMIT) && channel->getLimit() <= channel->getUser().size())
				throw ERR_OUTOFLIMIT();
		if ((channel->getGrant() & M_INVITE))
				throw ERR_INVITEONLYCHAN();
		channel->addUser(_clientFd);
		channel->setOperator(_clientFd, _clientFd);
		if (channel->getGrant() & M_KEY){
			client->addBackCarriageBuffer(client->getNickname() + ": JOIN " + channel->getName()+ " using key " + key);
			makeBufferString(channel, client);
		}
		else {
			client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName() + "\r\n");
			makeBufferString(channel, client);
		}
	} catch(IrcDB::ERR_CHANNEL_NOT_IN_DB &e){
		channel = new IrcChannel(name);
		channel->addUser(_clientFd);
		channel->setOperator(_clientFd, _clientFd);
		if (key.size() != 0){
			channel->setPassword(key);
			channel->setGrant(M_KEY, true);
			client->addBackBuffer(":" + client->getNickname() + " JOIN " + channel->getName()+ " using key " + key + "\r\n");
			makeBufferString(channel, client);
		}
		else{
			client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName()+ "\r\n");	
			makeBufferString(channel, client);
		}	
		_db->insertChannel(channel);
	} catch (std::exception &e){
		_db->findClientByFd(_clientFd)->addBackCarriageBuffer(e.what());
	}

}


void IrcCommand::JOIN(){
	std::deque<std::string> channelList;
	std::deque<std::string> passwordList;
	std::multimap<std::string, std::string>keypair;
	if (_args.size() != 2 && _args.size() != 1)
		throw ERR_NEEDMOREPARAMS();
	for (int end = _args[0].find(","); end != -1; end = _args[0].find(",")){
		channelList.push_back(_args[0].substr(0, end));
		_args[0].erase(0, end + 1);
	}
	channelList.push_back(_args[0]);
	if (channelList.back().size() == 0)
		channelList.pop_back();
	if (_args.size() == 2){
		for (int end = _args[1].find(","); end != -1; end = _args[1].find(",")){
			passwordList.push_back(_args[1].substr(0, end));
			_args[1].erase(0, end + 1);
		}
		passwordList.push_back(_args[1]);
		if (passwordList.back().size() == 0)
			passwordList.pop_back();
	}
	if (channelList.size() < passwordList.size())
		throw ERR_NEEDMOREPARAMS();
	for (int i = 0; i < channelList.size(); i++){
		if (i < passwordList.size())
			keypair.insert(std::make_pair(channelList[i], passwordList[i]));
		else
			keypair.insert(std::make_pair(channelList[i], ""));
	}
	for (std::multimap<std::string, std::string>::iterator it = keypair.begin(); it != keypair.end(); it++){
		joinChannel(it->first, it->second); 
	}
}