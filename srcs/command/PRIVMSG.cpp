/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:37 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/21 18:52:00 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcDB.hpp"

void IrcCommand::PRIVMSG(){
		IrcClient* client = _db->findClientByFd(_clientFd);
    if (_args.size() != 2)
    {
		client->addBackBuffer(":localhost 461 " + client->getNickname() + " PRIVMSG ");
        throw ERR_NEEDMOREPARAMS();
    }
    std::string msg = _args[1];

    std::deque<std::string> clientList;
	for (int end = _args[0].find(","); end != -1; end = _args[0].find(",")){
		clientList.push_back(_args[0].substr(0, end));
		_args[0].erase(0, end + 1);
	}
	clientList.push_back(_args[0]);
	if (clientList.back().size() == 0)
		clientList.pop_back();


    for (std::deque<std::string>::iterator it3 = clientList.begin(); it3 != clientList.end(); it3++){
        if (it3->at(0) == '#')
        {
            std::cout << "In PRIVMSG # start, so channel" << std::endl;
            IrcChannel* channel = _db->findChannel(*it3);
            std::map<int, bool>	users = channel->getUser();
            std::map<int, bool>::iterator it;

            if (!channel->isJoinedUser(_clientFd))
            {
                client->addBackBuffer(":localhost 442 " + client->getNickname() + " " + channel->getName());
                throw ERR_NOTONCHANNEL();
            }
            for (it = users.begin();
                    it != users.end();
                    it++)
                    {
                        IrcClient *client = _db->findClientByFd(_clientFd);
                        IrcClient *target = _db->findClientByFd(it->first);                    
                        if (_clientFd != target->getFd())
                        { 
                            target->addBackCarriageBuffer(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg);
                            client->addBackCarriageBuffer(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg);
                        }
                    }      
        }
        else
        {
                IrcClient *client = _db->findClientByFd(_clientFd);
                IrcClient *target = _db->findClientByName(*it3);        
            if (_clientFd != target->getFd())
            {
                target->addBackCarriageBuffer(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg);
                client->addBackCarriageBuffer(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg);
            }
        }
    }
}
