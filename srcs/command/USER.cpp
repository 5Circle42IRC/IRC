/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:45 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/29 18:57:05 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::USER(){

    IrcClient* client = _db->findClientByFd(_clientFd);
    std::string oldNick = client->getNickname();
    if (getArgs().size() != 4)
    {
        client->addBackBuffer(":localhost 461 * " + _command);
        throw ERR_NEEDMOREPARAMS();
    }
    else    
    {
        _db->findClientByFd(_clientFd)->setUsername(getArgs()[0]);
        _db->findClientByFd(_clientFd)->setHostname(getArgs()[1]);
        _db->findClientByFd(_clientFd)->setServername(getArgs()[2]);
        _db->findClientByFd(_clientFd)->setRealname(getArgs()[3]);


        client->addBackBuffer(":" + oldNick + " USER " + getArgs()[0] + " " + getArgs()[1] + " " + getArgs()[2] + " " + getArgs()[3] +  "\n");
    }
}
