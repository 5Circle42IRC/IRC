/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:06 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/29 18:41:44 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::kickUser(std::string channelName, std::string clientName, std::string comment){

	IrcChannel *channel;
	IrcClient *target;
	IrcClient *client;


	//operator 확인
	channel = _db->findChannel(channelName);
	if (channel->isOperator(_clientFd) == false)
	{
		client = _db->findClientByFd(_clientFd);
	    client->addBackBuffer(":localhost 481 " + client->getNickname() + " " + channel->getName());
		throw ERR_NOPRIVILEGES();
	}
	//channel 및 user 유무 확인
	target = _db->findClientByName(clientName);
	client = _db->findClientByFd(_clientFd);
	if (channel->isJoinedUser(target->getFd()) == false)
	{
		client->addBackBuffer(":localhost 442 " + client->getNickname() + " " + channel->getName());
		throw ERR_NOTONCHANNEL();
	}
	channel->setRemoveOperater(target->getFd());
	channel->deleteUser(target->getFd());

	if (comment.size() > 0)
	{
		target->addBackBuffer(":localhost 404 " + client->getNickname() + " KICK " + channel->getName() + " " + target->getNickname() + " :" + comment+ "\r\n");
		client->addBackBuffer(":localhost 404 " + client->getNickname() + " KICK " + channel->getName() + " " + target->getNickname() + " :" + comment+ "\r\n");
	}
	else
	{
		target->addBackBuffer(":localhost 404 " + client->getNickname() + " KICK " + channel->getName() + " " + target->getNickname()+ "\r\n");
		client->addBackBuffer(":localhost 404 " + client->getNickname() + " KICK " + channel->getName() + " " + target->getNickname() + " :" + comment+ "\r\n");
	}
	if (channel->getUser().size() == 0)
		_db->deleteChannel(channel->getName());	
}

void IrcCommand::KICK(){
	std::deque<std::string> channelList;
	std::deque<std::string> userList;
	int i = 0;

	if(_args.size() > 3 || _args.size() < 2)
	{
		IrcClient* client = _db->findClientByFd(_clientFd);
		client->addBackBuffer(":localhost 461 " + client->getNickname() + " KICK ");
		throw ERR_NEEDMOREPARAMS();
	}
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
	{
		IrcClient* client = _db->findClientByFd(_clientFd);
		client->addBackBuffer(":localhost 461 " + client->getNickname() + " KICK ");
		throw ERR_NEEDMOREPARAMS();
	}
	for (std::deque<std::string>::iterator it = channelList.begin(); it != channelList.end(); it++){
		if (_args.size() == 3)
			kickUser(*it, userList[i], _args[2]);
		else
			kickUser(*it, userList[i], "");
		i++;
	}
}
