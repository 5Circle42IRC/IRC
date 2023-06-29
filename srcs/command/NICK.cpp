/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:15 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/26 12:06:46 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

int IrcCommand::checkValidNICK(std::deque<std::string> args, IrcDB *_db)
{
    std::string newNick = args[0]; 
    std::string oldNick = _db->findClientByFd(_clientFd)->getNickname();
    IrcClient* client = _db->findClientByFd(_clientFd);
    if (newNick.size() > 9 || newNick == ":")
    {
        client->addBackBuffer(":localhost 432 " + client->getNickname() + " :Erroneous " + newNick);
        throw ERR_ERRONEUSNICKNAME();  
    }

    std::map<int, IrcClient*>clients = _db->getAllClients();
    std::map<int, IrcClient*>::iterator it;
    for (it = clients.begin(); it != clients.end(); it ++)
    {
        
        if (it->second->getNickname() == newNick)
        {        
            client->addBackBuffer(":localhost 433 * " + client->getNickname() + " :Nickname is already in use ");   
            throw ERR_NICKNAMEINUSE();
        }         
    }

    return 0;
}

void IrcCommand::NICK(){
        //std::cout << "start NICK" << std::endl;
        IrcClient* client = _db->findClientByFd(_clientFd);
        if (getArgs().size() != 1)
        {
            //std::cout << "no nick given" << std::endl;
            client->addBackBuffer(":localhost 431 " + client->getNickname() + " ");            
            throw ERR_NONICKNAMEGIVEN();               
        }
        else
        {
            //std::cout << "n1" << std::endl;
            std::string oldNick = client->getNickname();
            //std::cout << "n2" << std::endl;
            checkValidNICK(getArgs(), _db);
            //std::cout << "n3" << std::endl;
            _db->findClientByFd(_clientFd)->setNickname(getArgs()[0]);
            //std::cout << "n4" << std::endl;
            //:WiZ NICK Kilroy 
            client->addBackBuffer(":" + oldNick + " NICK :" + client->getNickname() + "\r\n");
            client->setPasswordFlag(true);
        }    
}
