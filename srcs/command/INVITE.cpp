/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:46:57 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/26 12:11:12 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::INVITE(){
	IrcClient *host = _db->findClientByFd(_clientFd);

	if (_args.size() != 2){
		host->addBackBuffer("433 <" + _command + "> ");
		throw ERR_NEEDMOREPARAMS();
	}

	IrcClient *target = _db->findClientByName(_args[0]);
	IrcChannel *channel = _db->findChannel(_args[1]);
	// 채널 안에 있는지 검사
	if (!channel->isJoinedUser(_clientFd)){
		host->addBackBuffer(":localhost 442 " + host->getNickname());
		throw ERR_NOTONCHANNEL();
	}
	// 채널 limit 검사
	if ((channel->getGrant() & M_LIMIT) && (static_cast<unsigned long>(channel->getLimit()) <= channel->getUser().size())){
		host->addBackBuffer(":localhost 471 " + host->getNickname() + " " + channel->getName());
		throw ERR_CHANNELISFULL();
	}

	//권한 검사
	if ((channel->getGrant() & M_INVITE) && !channel->isOperator(_clientFd)){
		host->addBackBuffer(":localhost 482 " + host->getNickname() + " " + channel->getName());
		throw ERR_CHANOPRIVSNEEDED();
	}
	//이미 존재하는 유저인지 검사
	if (channel->isJoinedUser(target->getFd())){
		host->addBackBuffer(":localhost 443 " + target->getNickname());
		throw ERR_USERONCHANNEL();
	}
	channel->addUser(target->getFd());
	
	host->addBackCarriageBuffer(":" + host->getNickname() + " INVITE " + target->getNickname() + " " + channel->getName());
	target->addBackCarriageBuffer(":" + host->getNickname() + " INVITE " + target->getNickname() + " " + channel->getName());
	//가입한 채널 정보 띄워주기
	std::string joinedUserList;
	std::map<int, bool> userList = channel->getUser();
	for (std::map<int, bool>::iterator it = userList.begin(); it != userList.end(); it++){
		joinedUserList += (_db->findClientByFd(it->first)->getNickname() + ", ");
	}
	target->addBackCarriageBuffer("<Channel Info>");
	target->addBackCarriageBuffer("< Name       : " + channel->getName() + " >");
	target->addBackCarriageBuffer("< Topic      : " + channel->getTopic() + " >");
	target->addBackCarriageBuffer("< joinedUser : " + joinedUserList + " >");
}
