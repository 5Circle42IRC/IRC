/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcDB.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysungwon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:45:25 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/21 18:45:27 by ysungwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRC_DB_HPP__
#define __IRC_DB_HPP__

#include <iostream>
#include <map>
#include <utility>

class IrcClient;
class IrcChannel;

class IrcDB
{
public:
    IrcDB();
    ~IrcDB();

	IrcClient *findClientByName(std::string name);
	IrcClient *findClientByFd(int clientFd);

	IrcChannel *findChannel(std::string name);

	void insertClient(IrcClient *client);
	void insertChannel(IrcChannel *channel);

	void deleteClient(int clientFd);
	void deleteChannel(std::string name);
	void setServPass(const std::string& pass);

	const std::map<std::string, IrcChannel*> getAllChannels() const;
	const std::map<int, IrcClient*> getAllClients() const;
	const std::string& getServPass() const;

	class ERR_NOSUCHCHANNEL: public std::exception{
		virtual const char *what() const throw();
	};
	class ERR_NOSUCHCLIENTFD: public std::exception{
		virtual const char *what() const throw();
	};
	class ERR_NOSUCHNICK : public std::exception {
		virtual const char *what() const throw();
	};		

private:
	std::map<std::string , IrcChannel*> _channels;
	std::map<int, IrcClient *> _clients;
	std::string _servPass;
};

#endif
