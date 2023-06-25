/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:02 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/26 08:01:56 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"
#include <algorithm>
#include <stdio.h>

void IrcCommand::joinChannel(std::string name, std::string key){
	IrcChannel *channel;
	IrcClient *client = _db->findClientByFd(_clientFd);
	if (name.at(0) != '#'){
		client->addBackBuffer("432 <" + name + "> ");
		throw ERR_BADCHANNELNAME();
		}
	if (name.size() > 200){
		client->addBackBuffer("432 <" + name + "> ");
		throw ERR_BADCHANNELNAME();
	}
	try {
		channel =_db->findChannel(name);
		if (channel->isJoinedUser(_clientFd)){
			client->addBackBuffer("443 <" + client->getNickname() + "> <" + channel->getName() + "> ");
			throw ERR_USERONCHANNEL();
		}
		// grant 검사(mode)
		if ((channel->getGrant() & M_KEY) && channel->getPassword().compare(key)){
			client->addBackBuffer("475 <" + channel->getName() + "> ");
			throw ERR_BADCHANNELKEY();
		}
		if ((channel->getGrant() & M_LIMIT) && static_cast<unsigned long>(channel->getLimit()) <= channel->getUser().size()){
			client->addBackBuffer("471 <" + channel->getName() + "> ");
			throw ERR_CHANNELISFULL();
		}
		if ((channel->getGrant() & M_INVITE)){
			client->addBackBuffer("473 <" + channel->getName() + "> ");
			throw ERR_INVITEONLYCHAN();
		}

		// channel에 추가 후 모든 참가자에게 알림
		channel->addUser(_clientFd);
		channel->setOperator(_clientFd, _clientFd);
		std::map<int, bool> userList = channel->getUser();
		std::string userListStr;
		for (std::map<int,bool>::iterator it = userList.begin(); it != userList.end(); it++){
			IrcClient *target = _db->findClientByFd(it->first);
			if (it->second == true)
				userListStr += ("@" + target->getNickname() + " ");
			else
				userListStr += (target->getNickname() + " ");
			target->addBackCarriageBuffer(":" + client->getNickname() + " JOIN :" + channel->getName());
		}
		userListStr.pop_back();
		std::string mode = "+";
		if (channel->getGrant() & M_INVITE)
			mode += "i";
		else if (channel->getGrant() & M_KEY)
			mode += "k";
		else if (channel->getGrant() & M_LIMIT)
			mode += "l";
		else if (channel->getGrant() & M_TOPIC)
			mode += "t";
		client->addBackCarriageBuffer(":localhost 332 " + client->getNickname() + " " + channel->getName() + " :" + channel->getTopic());
		client->addBackCarriageBuffer(":localhost 324 " + client->getNickname() + " " + channel->getName() + " " + mode);
		client->addBackCarriageBuffer(":localhost 353 " + client->getNickname() + " =" + channel->getName() + " :" + userListStr);
		client->addBackCarriageBuffer(":localhost 366 " + client->getNickname() + " " + channel->getName() + " :End of NAMES list");
	} catch(std::string name){
		// channel 생성 후 클라이언트에 알림
		channel = new IrcChannel(name);
		channel->addUser(_clientFd);
		channel->setOperator(_clientFd, _clientFd);
		if (key.size() != 0){
			channel->setPassword(key);
			channel->setGrant(M_KEY, true);
		}
		client->addBackCarriageBuffer(":" + client->getNickname() + "!~" + client->getNickname() + "@localhost JOIN :" + channel->getName());
		_db->insertChannel(channel);
	} catch (std::exception &e){
		_db->findClientByFd(_clientFd)->addBackCarriageBuffer(e.what());
	}
}


void IrcCommand::JOIN(){
	IrcClient *client = _db->findClientByFd(_clientFd);
	std::deque<std::string> channelList;
	std::deque<std::string> passwordList;
	std::multimap<std::string, std::string>keypair;
	if (_args.size() != 2 && _args.size() != 1){
		client->addBackBuffer("461 <" + _command + "> ");
		throw ERR_NEEDMOREPARAMS();
	}
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
	if (channelList.size() < passwordList.size()){
		client->addBackBuffer("461 <" + _command + "> ");
		throw ERR_NEEDMOREPARAMS();
	}
	for (unsigned long i = 0; i < channelList.size(); i++){
		if (i < passwordList.size())
			keypair.insert(std::make_pair(channelList[i], passwordList[i]));
		else
			keypair.insert(std::make_pair(channelList[i], ""));
	}
	for (std::multimap<std::string, std::string>::iterator it = keypair.begin(); it != keypair.end(); it++){
		joinChannel(it->first, it->second); 
	}
}
