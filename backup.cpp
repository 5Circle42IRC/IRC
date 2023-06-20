void IrcCommand::INVITE(){
	if (_args.size() != 2)
		throw ERR_NEEDMOREPARAMS();

	IrcClient *host = _db->findClientByFd(_clientFd);
	IrcClient *target = _db->findClientByName(_args[1]);
	IrcChannel *channel = _db->findChannel(_args[0]);

	// 채널의 리미트가 걸려있음
	if (channel->getLimit() <= channel->getUser().size()){
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