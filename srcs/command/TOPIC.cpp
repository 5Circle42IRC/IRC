/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysungwon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:41 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/21 18:47:42 by ysungwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcChannel.hpp"

void IrcCommand::TOPIC(){
     
    if (_args.size() > 2)
    {
		IrcClient* client = _db->findClientByFd(_clientFd);
		client->addBackBuffer(":localhost 461 " + client->getNickname() + " TOPIC ");
        throw ERR_NEEDMOREPARAMS();
    }

    if (_args.size() == 0)
    {
		IrcClient* client = _db->findClientByFd(_clientFd);
		client->addBackBuffer(":localhost 461 " + client->getNickname() + " TOPIC ");
        throw ERR_NEEDMOREPARAMS();
    }    
    std::string chname = _args[0];
    std::string topic = _args[1];
    IrcChannel* channel = _db->findChannel(chname);
    IrcClient* client = _db->findClientByFd(_clientFd);

    if (channel->isJoinedUser(_clientFd) == false)
    {
        client->addBackBuffer(":localhost 442 " + client->getNickname() + " " + channel->getName());
        throw ERR_NOTONCHANNEL();
    }

    if (_args.size() == 1)
    {
        client->addBackBuffer(":"+client->getNickname() +" 332 " + client->getNickname() + " " + channel->getName()+ " " + channel->getTopic() +"\r\n");
    }
    else if (_args.size() == 2)
    {
        if (  (channel->getGrant() & M_TOPIC) && (channel->isOperator(_clientFd) == false))
        {
            client->addBackBuffer(":localhost 482 " + client->getNickname() + " " + channel->getName());
            throw ERR_CHANOPRIVSNEEDED();

        }
        else
        {

            channel->setTopic(topic);
            if (channel->getTopic().size() == 0)//채널 생성시 토픽의 사이즈는???
                {
                    client->addBackBuffer(client->getNickname() + " " + channel->getName() + " :No topic is set" + "\r\n");
                }
            else
                {
                    client->addBackBuffer(":" + client->getNickname() + " TOPIC " + client->getNickname() + " :" + channel->getName()+ " " + topic +"\r\n");
                
                }
        }
    }
}
