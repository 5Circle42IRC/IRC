#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

//다중 join 로직 필요함 -> 파서에서 해결


static void joinChannel(IrcDB *_db, const std::string& name, const std::string& key, int clientFd) {

	IrcChannel *channel;
	IrcClient *client = _db->findClientByFd(clientFd);
	//인자 개수 확인 (오류시 throw invalid error)
	try {
		channel =_db->findChannel(name);
	} catch(std::exception &e){
		channel = new IrcChannel(name);
		channel->addUser(clientFd);
		channel->setGrant(OPERATOR, 1);
		channel->setOperator(clientFd, clientFd);
		_db->insertChannel(channel);
	}
	channel->addUser(clientFd);
	client->addBackBuffer(client->getNickname() + ": JOIN " + channel->getName());
}

void IrcCommand::JOIN(int clientFd){
	for(int i = 0; i < _args.size() / 2; i++){
		
	}

}