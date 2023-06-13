#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::JOIN(int clientFd) {
	IrcChannel *channel;
	IrcClient *client = _db->findClientByFd(clientFd);
	//인자 개수 확인 (오류시 throw invalid error)
	try {
		channel =_db->findChannel(_args[0]);
	} catch(std::exception &e){
		channel = new IrcChannel(_args[0]);
		channel->addUser(clientFd);
		channel->setGrant(OPERATOR, 1);
		channel->setOperator(clientFd, clientFd);
		_db->insertChannel(channel);
	}
	channel->addUser(clientFd);
	client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName());
}