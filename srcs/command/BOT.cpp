#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"
#include <map>
#include <sstream>
void IrcCommand::BOT(){
	IrcClient *client = _db->findClientByFd(_clientFd);
	if(_args[0] == ":Cheers" && _args.size() == 1)
		client->addBackCarriageBuffer(":bot PRIVMSG " + client->getNickname() + " :Cheers to good times!");
	else if (_args[0] == ":echo" && _args.size() == 2)
		client->addBackCarriageBuffer(":bot PRIVMSG " + client->getNickname() + " :" + _args.back());
	else if (_args[0] == ":chInfo" && _args.size() == 1){
		std::map<std::string, IrcChannel *> channelList = _db->getAllChannels();
		if (channelList.size() == 0)
			client->addBackCarriageBuffer(":localhost 323 " + client->getNickname() + " :End of /List");
		for (std::map<std::string, IrcChannel *>::iterator it = channelList.begin(); it != channelList.end(); it++){
			IrcChannel *channel = _db->findChannel(it->first);
			std::ostringstream oss;
			oss << channel->getUser().size();
			std::string num = oss.str();
			std::string mode = "+";
			if (channel->getGrant() & M_INVITE)
				mode += "i";
			if (channel->getGrant() & M_KEY)
				mode += "k";
			if (channel->getGrant() & M_LIMIT)
				mode += "l";
			if (channel->getGrant() & M_TOPIC)
				mode += "t";
			if (channel->isOperator(_clientFd))
				mode += "@";
			else if (channel->isJoinedUser(_clientFd))
				mode += "=";
			client->addBackCarriageBuffer(":localhost 322 " + client->getNickname() + " " + channel->getName() + " " + num + " " + mode + " :" + channel->getTopic());
		}
	}
	else if (_args[0] == ":chInfo" && _args.size() == 2 && _args[1].at(0) == '#'){
		IrcChannel *channel = _db->findChannel(_args[1]);
		std::ostringstream oss;
		oss << channel->getUser().size();
		std::string num = oss.str();
		std::string mode = "+";
		if (channel->getGrant() & M_INVITE)
			mode += "i";
		if (channel->getGrant() & M_KEY)
			mode += "k";
		if (channel->getGrant() & M_LIMIT)
			mode += "l";
		if (channel->getGrant() & M_TOPIC)
			mode += "t";
		if (channel->isOperator(_clientFd))
			mode += "@";
		else if (channel->isJoinedUser(_clientFd))
			mode += "=";
		client->addBackCarriageBuffer(":localhost 322 " + client->getNickname() + " " + channel->getName() + " " + num + " " + mode + " :" + channel->getTopic());
	}
	else if (_args[0] == ":whoIs" && _args.size() == 2){
		std::map<int, IrcClient *> userList = _db->getAllClients();
		IrcClient *target = _db->findClientByName(_args[1]);
		for (std::map<int, IrcClient *>::iterator it = userList.begin(); it != userList.end(); it++){
			client->addBackCarriageBuffer(":localhost 311 " + client->getNickname() + " " + target->getNickname() + " " + target->getUsername() + " " + target->getHostname() + " * :" + target->getRealname());
		}
	}
	else
		_db->findClientByFd(_clientFd)->addBackCarriageBuffer(":localhost 421 " + _db->findClientByFd(_clientFd)->getNickname() + " " + _command + " :Unknown command");		
}