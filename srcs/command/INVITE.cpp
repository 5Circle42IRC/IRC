/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:46:57 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/29 18:48:43 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::INVITE(){
	IrcClient *host = _db->findClientByFd(_clientFd);

	if (_args.size() != 2){
		host->addBackBuffer(":localhost 461 " + host->getNickname() + " INVITE ");
		throw ERR_NEEDMOREPARAMS();
	}

	IrcClient *target = _db->findClientByName(_args[0]);
	IrcChannel *channel = _db->findChannel(_args[1]);
	// 채널 안에 있는지 검사
	if (!channel->isJoinedUser(_clientFd)){
		host->addBackBuffer(":localhost 442: " + host->getNickname() + " " + channel->getName());
		throw ERR_NOTONCHANNEL();
	}
	if (channel->isJoinedUser(target->getFd())){
		host->addBackBuffer(":localhost 443 " + host->getNickname() + " " + target->getNickname() + " " + channel->getName());
		throw ERR_USERONCHANNEL();
	}
	// 채널 limit 검사
	if ((channel->getGrant() & M_LIMIT) && (static_cast<unsigned long>(channel->getLimit()) <= channel->getUser().size())){
		host->addBackBuffer(":localhost 471 " + host->getNickname() + " " + channel->getName());
		throw ERR_CHANNELISFULL();
	}

	//권한 검사
	if ((channel->getGrant() & M_LIMIT) && (static_cast<unsigned long>(channel->getLimit()) <= channel->getUser().size())){
		host->addBackBuffer(":localhost 471 " + host->getNickname() + " " + channel->getName());
		throw ERR_CHANNELISFULL();
	}
	//이미 존재하는 유저인지 검사
	if (channel->isJoinedUser(target->getFd())){
		host->addBackBuffer(":localhost 443 " + target->getNickname());
		throw ERR_USERONCHANNEL();
	}
	channel->addUser(target->getFd());
	
	host->addBackCarriageBuffer(":localhost 341 " + host->getNickname() + " " + channel->getName() + " " + target->getNickname());
	target->addBackCarriageBuffer(":localhost 341 " + target->getNickname() + " " + channel->getName() + " " + target->getNickname());
	//가입한 채널 정보 띄워주기
	std::string joinedUserList;
	std::map<int, bool> userList = channel->getUser();
	for (std::map<int, bool>::iterator it = userList.begin(); it != userList.end(); it++){
		joinedUserList += (_db->findClientByFd(it->first)->getNickname() + ", ");
	}
	std::string userListStr = " :";
	for (std::map<int,bool>::iterator it = userList.begin(); it != userList.end(); it++){
		IrcClient *target = _db->findClientByFd(it->first);
		if (it->second == true)
			userListStr += ("@" + target->getNickname() + " ");
		else
			userListStr += (target->getNickname() + " ");
		target->addBackCarriageBuffer(":" + target->getNickname() + " JOIN :" + channel->getName());
	}
	userListStr.pop_back();
	std::string mode = "+";
	if (channel->getGrant() & M_INVITE)
		mode += "i";
	if (channel->getGrant() & M_KEY)
		mode += "k";
	if (channel->getGrant() & M_LIMIT)
		mode += "l";
	if (channel->getGrant() & M_TOPIC)
		mode += "t";
	target->addBackCarriageBuffer(":localhost 332 " + target->getNickname() + " " + channel->getName() + " :" + channel->getTopic());
	target->addBackCarriageBuffer(":localhost 353 " + target->getNickname() + " =" + channel->getName() + userListStr);
}
