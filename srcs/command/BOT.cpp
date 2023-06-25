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
		for (std::map<std::string, IrcChannel *>::iterator it = channelList.begin(); it != channelList.end(); it++){
			IrcChannel *channel = _db->findChannel(it->first);
			std::ostringstream oss;
			oss << channel->getUser().size();
			std::string num = oss.str();
			std::string mode = "+";
			if (channel->getGrant() & M_INVITE)
				mode += "i";
			else if (channel->getGrant() & M_KEY)
				mode += "k";
			else if (channel->getGrant() & M_LIMIT)
				mode += "l";
			else if (channel->getGrant() & M_TOPIC)
				mode += "t";
			if (channel->isOperator(_clientFd))
				mode += "@";
			else if (channel->isJoinedUser(_clientFd))
				mode += "=";
			client->addBackCarriageBuffer(":127.0.0.1 322 " + client->getNickname() + " " + channel->getName() + " " + num + " " + mode + " :" + channel->getTopic());
		}
	}
}