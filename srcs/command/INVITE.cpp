#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::INVITE(){
	if (_args.size() != 2)
		throw ERR_INVALID_ARGUMENT();

	std::cout << "_args list : " << "<" << _args[0] << "> " << "<" << _args[1] << ">" << std::endl;
	IrcClient *host = _db->findClientByFd(_clientFd);
	IrcClient *target = _db->findClientByName(_args[0]);
	IrcChannel *channel = _db->findChannel(_args[1]);

	// 채널 limit 검사
	if ((channel->getGrant() & M_LIMIT) && (channel->getLimit() <= channel->getUser().size()))
		host->addBackCarriageBuffer("The channel is full");
	//권한 검사
	if ((channel->getGrant() & M_INVITE) && !channel->isOperator(_clientFd))
		throw ERR_NOT_OPERATOR();
	//이미 존재하는 유저인지 검사
	if (channel->isJoinedUser(target->getFd()))
		throw ERR_USER_ON_CHANNEL();
	channel->addUser(target->getFd());
	host->addBackCarriageBuffer(":" + host->getNickname() + " INVITE " + target->getNickname() + " " + channel->getName());
	target->addBackCarriageBuffer(":" + host->getNickname() + " INVITE " + target->getNickname() + " " + channel->getName());
}