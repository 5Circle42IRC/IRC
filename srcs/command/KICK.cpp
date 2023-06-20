#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::kickUser(std::string channelName, std::string clientName, std::string comment){

	std::cout << "1" << std::endl;
	IrcChannel *channel;
	IrcClient *target;
	//operator 확인
	std::cout << "2" << std::endl;
	channel = _db->findChannel(channelName);
	if (channel->isOperator(_clientFd) == false)
		throw ERR_NOT_OPERATOR();
	//channel 및 user 유무 확인
	target = _db->findClientByName(clientName);
	std::cout << "3" << std::endl;
	if (channel->isJoinedUser(target->getFd()) == false)
		throw ERR_NOT_ON_CHANNEL();
	channel->deleteUser(target->getFd());
	std::cout << "4" << std::endl;
	if (comment.size() > 0)
		target->addBackCarriageBuffer("kick " + target->getNickname() + " from " + channel->getName() + " using \"" + comment + "\" as the reason.");
	else
		target->addBackCarriageBuffer("kick " + target->getNickname() + " from " + channel->getName());

}

void IrcCommand::KICK(){
	std::deque<std::string> channelList;
	std::deque<std::string> userList;
	int i = 0;

	if(_args.size() > 3 || _args.size() < 2)
		throw ERR_INVALID_ARGUMENT();
	for (int end = _args[0].find(","); end != -1; end = _args[0].find(",")){
		channelList.push_back(_args[0].substr(0, end));
		_args[0].erase(0, end + 1);
	}
	channelList.push_back(_args[0]);
	if (channelList.back().size() == 0)
		channelList.pop_back();
	for (int end = _args[1].find(","); end != -1; end = _args[1].find(",")){
		userList.push_back(_args[1].substr(0, end));
		_args[1].erase(0, end + 1);
	}
	userList.push_back(_args[1]);
	if (userList.back().size() == 0)
		userList.pop_back();
	if (userList.size() != channelList.size())
		throw ERR_INVALID_ARGUMENT();
	for (std::deque<std::string>::iterator it = channelList.begin(); it != channelList.end(); it++){
		if (_args.size() == 3)
			kickUser(*it, userList[i], _args[2]);
		else
			kickUser(*it, userList[i], "");
		i++;
	}
}