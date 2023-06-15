#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

#include <string>
void IrcCommand::INVITE(){
	if (_args.size() != 2)
		throw ERR_INVALID_ARGUMENT();

	IrcClient *host;

	IrcChannel *channel = _db->findChannel(_args[1]);
	host = _db->findClientByFd(_clientFd);
	IrcClient *client = _db->findClientByName(_args[2]);

	// 채널의 리미트가 걸려있음
	if (channel->getLimit() == channel->getUser().size())
	{
		_db->findClientByFd(_clientFd)->addBackBuffer("Channel member is Full\r\n");
		return ; //throw 로 limit 에러 날릴 필요가 있음
	}

	// 권한 체크 및 오퍼레이터인지
	if (!(channel->getGrant() & M_INVITE) && channel->getUser().at(client->getFd()) == false)
		return ; // throw 로 권한 에러코드 날릴 필요가 있음.
	if (channel->getUser().find(client->getFd()) == channel->getUser().end())
		return; //throw 같은 아이디 에러코드
	channel->addUser(client->getFd());
	host->addBackBuffer("341\r\n");
	// dan-!d@localhost INVITE Wiz #test
	client->addBackBuffer(client->getNickname() + " INVITE " + host->getNickname() + " " + channel->getName() + "\r\n"); 
}
/*
	Command: INVITE
	Parameters: <nickname> <channel>	
	INVITE Wiz #foo_bar 
*/