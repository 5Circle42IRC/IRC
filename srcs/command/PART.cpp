/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:23 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/26 08:07:35 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"



void IrcCommand::PART(){
    IrcChannel *channel;
    IrcClient *client;

    std::vector<std::string> channelList;

    if (_args.size() < 1)
    {
		IrcClient* client = _db->findClientByFd(_clientFd);
		client->addBackBuffer(":localhost 461 " + client->getNickname() + " PART ");
        throw ERR_NEEDMOREPARAMS();
    }

    for (int end = _args[0].find(","); end != -1; end = _args[0].find(",")){
		channelList.push_back(_args[0].substr(0, end));
		_args[0].erase(0, end + 1);
	}
	channelList.push_back(_args[0]);

    

    client = _db->findClientByFd(_clientFd);

    std::string chname;
    std::string chnameSum;

    for (std::vector<std::string>::iterator it = channelList.begin(); it != channelList.end(); it++){
       
        channel =_db->findChannel(*it);
        chname = channel->getName();
        std::map<int, bool> userList = channel->getUser();
        /*
            ERR_NOSUCHCHANNEL (403) 
            "<client> <channel> :No such channel"        
        */



        int size = chname.size();
        (void)size;

        if (channel->isJoinedUser(_clientFd) == false)
        {
            client->addBackBuffer(":localhost 442 " + client->getNickname() + " " + channel->getName());
            throw ERR_NOTONCHANNEL();
        }              

        if (channel->deleteUser(_clientFd) == false)
        {
            throw ERR_NOTONCHANNEL();
        }

        chnameSum += (chname + " ");
        client->addBackBuffer(":" + client->getNickname() + " PART :" + chname + "\r\n");
        for (std::map<int, bool>::iterator useriter = userList.begin(); useriter != userList.end(); useriter++){
            IrcClient *target = _db->findClientByFd(useriter->first);
            target->addBackCarriageBuffer(":" + client->getNickname() + " PART :" + channel->getName());
        }        
        if (channel->getUser().size() == 1){
            channel->setOperator(-1, channel->getUser().begin()->first);
        }
        if (channel->getUser().size() == 0)
            _db->deleteChannel(channel->getName());        
    }
    return ;
}
