/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:11 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/26 13:23:49 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcChannel.hpp"

#include <sstream>

void IrcCommand::MODE()
{
    IrcClient *client = _db->findClientByFd(_clientFd);

    
    
    
    //size check
    if (_args.size() < 2)
    {
        return;
    }
    //channel find check
    IrcChannel* channel = _db->findChannel(_args[0]);
    
    if (channel->isJoinedUser(_clientFd) == false)
    {
        client->addBackBuffer(":localhost 442 " + client->getNickname() + " " + channel->getName());
        throw ERR_NOTONCHANNEL();
    }
    //channel operator check
    if (channel->isOperator(_clientFd) == false)
    {
        client->addBackBuffer(":localhost 482 " + client->getNickname() + " " + channel->getName());
        throw ERR_CHANOPRIVSNEEDED();
    }

    std::string option = _args[1];
    if (option.size() < 2){
        client->addBackBuffer(":localhost 461 " + client->getNickname() + " " + _command);
        throw ERR_NEEDMOREPARAMS();
    }

    int mustOp = 0;
    int sign = 1;
    


    int argCount = 2;
    int i = 0;
    while(option[i])
    {

        if (option[i] == '+')
        {
            if (mustOp==1)
            {
                client->addBackBuffer(":localhost 472 " + client->getNickname());
                throw ERR_UNKNOWNMODE();
            }                  
            sign = 1;
            mustOp = 1; //we have +, mustOP is setted to 1, then, next char have to be option char.
        }
        else if (option[i] == '-')
        {
            if (mustOp==1)
            {
                client->addBackBuffer(":localhost 472 " + client->getNickname());
                throw ERR_UNKNOWNMODE();
            }                 
            sign = 0;
            mustOp = 1;//we have -, mustOP is setted to 1, then, next char have to be option char.
        }
        else
        {
            mustOp = 0;
            if (option[i] == '+' || option[i] == '-')
            {
                client->addBackBuffer(":localhost 472 " + client->getNickname());
                throw ERR_UNKNOWNMODE();  
            }
            else if (option[i] == 'i')
            {
                channel->setGrant(M_INVITE, sign);
            }          
            else if (option[i] == 't')
            {
                channel->setGrant(M_TOPIC, sign);
            }
            else if (option[i] == 'k')
            {
                channel->setGrant(M_KEY, sign);
                if (sign && static_cast<int> (_args.size()) > argCount)
                    channel->setPassword(_args[argCount++]);
            }
            else if (option[i] == 'l')
            {
                channel->setGrant(M_LIMIT, sign);
                if (sign && static_cast<int> (_args.size()) > argCount)
                {
                    int tmp = 0;
                    std::stringstream ssInt(_args[argCount++]);
                    ssInt >> tmp;
                    if (tmp < 1){
                        client->addBackBuffer(":localhost 501 " + client->getNickname());
                        throw ERR_INVALIDMODEVALUE();        
                    }      
                    channel->setLimit(tmp);
                }
            }
            else if (option[i] == 'o')
            {
                if (sign)
                {
                    if (static_cast<int> (_args.size()) > argCount)
                    {
                        int target = _db->findClientByName(_args[argCount++])->getFd();
                        if (channel->isJoinedUser(target))
                        {
                            channel->setOperator(_clientFd, target);
                        }
                    }
                }
                else
                {
                    if (static_cast<int> (_args.size()) > argCount)
                    {                    
                        int target = _db->findClientByName(_args[argCount++])->getFd();
                        if (channel->isJoinedUser(target))                    
                        {
                            channel->deleteOperator(_clientFd, target);
                        }
                    }
                }
            }
            else
            {
                client->addBackBuffer(":localhost 472 " + client->getNickname());
                throw ERR_UNKNOWNMODE();
            }  
        }
        i++;
    }
    std::string stringSum;
    std::deque<std::string>::iterator it;
    for (it = _args.begin(); it!= _args.end(); it++)
        stringSum += *it + " ";
    client->addBackCarriageBuffer(":" + client->getNickname() + " MODE " + channel->getName() + " " + stringSum);
}
