/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PONG.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysungwon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:32 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/21 18:47:34 by ysungwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::PING(){
    std::cout << "IN PING" << std::endl;
    IrcClient* client = _db->findClientByFd(_clientFd);
    client->addBackBuffer(client->getNickname() + " PONG" + "\r\n");
}
