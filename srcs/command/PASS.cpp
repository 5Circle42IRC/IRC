/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysungwon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:27 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/21 18:47:29 by ysungwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::PASS(){
    
        IrcClient* client = _db->findClientByFd(_clientFd);

        if (getArgs().size() != 1)
        {
            IrcClient* client = _db->findClientByFd(_clientFd);
		    client->addBackBuffer(":localhost 461 " + client->getNickname() + " PASS ");
            throw ERR_NEEDMOREPARAMS();               
        }
        else
        {
            client->addBackBuffer(":" + client->getNickname() + " PASS " + _args[0] + "\r\n");
        }    
}
