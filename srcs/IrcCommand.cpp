/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:46:39 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/25 13:26:52 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/IrcCommand.hpp"
#include "../include/IrcClient.hpp"
#include <sstream>

IrcCommand::IrcCommand(IrcDB *db, int clientFd): _db(db), _clientFd(clientFd) {
	_commandList["INVITE"] = &IrcCommand::INVITE;
	_commandList["JOIN"] = &IrcCommand::JOIN;
	_commandList["NICK"] = &IrcCommand::NICK;
	_commandList["PART"] = &IrcCommand::PART;
	_commandList["PING"] = &IrcCommand::PING;
	_commandList["PRIVMSG"] = &IrcCommand::PRIVMSG;
	_commandList["TOPIC"] = &IrcCommand::TOPIC;
	_commandList["USER"] = &IrcCommand::USER;
	_commandList["MODE"] = &IrcCommand::MODE;
	_commandList["DISPLAY"] = &IrcCommand::DISPLAY;
	_commandList["KICK"] = &IrcCommand::KICK;
	_commandList["PASS"] = &IrcCommand::PASS;
}
IrcCommand::~IrcCommand(){}

void IrcCommand::checkRunCMD(){
	std::map<std::string, commandPtr>::iterator commandIter;
	for (commandIter = _commandList.begin(); commandIter != _commandList.end(); commandIter++){
		if (commandIter->first == _command){
			commandPtr funcPtr = commandIter->second;
			(this->*funcPtr)();
			return ;
		}
	}
	_db->findClientByFd(_clientFd)->addBackCarriageBuffer("ERR_UNKNOWNCOMMAND\n");
}
void IrcCommand::login(IrcClient *client){

	if (client->getPasswordFlag() == 0){
		if (_command == "PASS" && _args.size() == 1 && _db->getServPass() == _args.front()){
			checkRunCMD();
			client->setPasswordFlag(1);
			client->addBackCarriageBuffer("\033[38;5;3minput your nickname using NICK command \033[0m");
			return ;
		}
		else {
			client->addBackCarriageBuffer("\033[38;5;3minput server password using PASS command \033[0m");
			return;
		}
	}
	if (client->getPasswordFlag() == 1 ){
		if (_command == "NICK" && _args.size() == 1){
			try{
				checkRunCMD();
				client->setPasswordFlag(2);
				client->addBackCarriageBuffer("\033[38;5;3minput your nickname using USER command \033[0m");
				return ;
			} catch (std::exception &e){
				client->addBackCarriageBuffer("\033[38;5;3minput your nickname using NICK command \033[0m");
				return ;
			}
		}
		else {
			client->addBackCarriageBuffer("\033[38;5;3minput your nickname using NICK command \033[0m");
			return ;
		}
	}
	if (client->getPasswordFlag() == 2){
		if (_command == "USER" && _args.size() == 4){
			try {
				checkRunCMD();
				client->setPasswordFlag(3);
				client->addBackCarriageBuffer("001 " + client->getNickname() + " :Welcome to Internet Relay Chat By ysungwon, juha, jwee");
				return ;
			} catch (std::exception &e){
				client->addBackCarriageBuffer("\033[38;5;3minput your userinfo using USER command \033[0m");
				return ;
			}
		}
		else {
			client->addBackCarriageBuffer("\033[38;5;3minput your userinfo using USER command \033[0m");
			return ;
		}
	}
}
void IrcCommand::makeCommand(std::string message){
	std::stringstream msg(message);
	std::string temp;

	if (message.size() > 512)
		throw ERR_OUTOFBOUNDMESSAGE();
	msg >> _command;
	
	if (_command == "PRIVMSG" || _command == "TOPIC" || _command == "KICK") {
		
		msg >> temp;
		_args.push_back(temp);
		if (_command == "KICK")
		{
			msg >> temp;
			_args.push_back(temp);
		}
		std::string temp2;
		while (msg >> temp){
			temp2 += temp;
			temp2 += " ";
		}		
		if (temp2.size() != 0)
		{
			_args.push_back(temp2);
			_args.back().pop_back();
		}
	}
	else {
		while (msg >> temp)
			_args.push_back(temp);
	}
}
void IrcCommand::parsing(std::string message){

	IrcClient *client = _db->findClientByFd(_clientFd);

	makeCommand(message);
	if (client->getPasswordFlag() < 3){
		login(client);
		return ;
	}
	try {
		checkRunCMD();
	} catch (std::string name) {
		client->addBackCarriageBuffer("403 <" + name + "> :No such channel");
	} catch (char *name){
		std::string temp = name;
		client->addBackBuffer("401 <" + temp + "> :No such nick/channel");
	}
		catch (std::exception &e){
		client->addBackCarriageBuffer(e.what());
	}
}








std::deque<std::string>& IrcCommand::getArgs(){ return _args; }
std::string IrcCommand::getCommand(){ return _command; }
IrcCommand& IrcCommand::setClientFd(int clientFd){ _clientFd = clientFd; return *this; }

//JOIN
const char* IrcCommand::ERR_BADCHANNELKEY::what() const throw() { return " :cannot join channel (+k)"; }
const char* IrcCommand::ERR_USERONCHANNEL::what() const throw() { return " :is already on channel"; }
const char* IrcCommand::ERR_NEEDMOREPARAMS::what() const throw() { return " :Not enough parameters"; }
const char* IrcCommand::ERR_CHANNELISFULL::what() const throw() { return " :cannot join channel (+l)";}
const char* IrcCommand::ERR_INVITEONLYCHAN::what() const throw() { return " :cannot join channel (+i)";}

//PARSING
const char* IrcCommand::ERR_UNKNOWNCOMMAND::what() const throw() { return " :Unknown command";}
const char* IrcCommand::ERR_OUTOFBOUNDMESSAGE::what() const throw() { return " :command is too long"; }
const char* IrcCommand::ERR_BADCHANNELNAME::what() const throw() { return " :invalid channel name";}

//NICK
const char* IrcCommand::ERR_NICKNAMEINUSE::what() const throw() { return " :Nickname is already in use"; }
const char* IrcCommand::ERR_NONICKNAMEGIVEN::what() const throw() { return " :No nickname given"; }
const char* IrcCommand::ERR_ERRONEUSNICKNAME::what() const throw() { return " :Erroneus nickname"; }
//PART
const char* IrcCommand::ERR_NOTONCHANNEL::what() const throw() { return " :You're not on that channel"; }
//TOPIC
const char* IrcCommand::ERR_CHANOPRIVSNEEDED::what() const throw() { return " :You're not channel operator"; }

//MODE
const char* IrcCommand::ERR_UNKNOWNMODE::what() const throw() { return " :is unknwon mode char to me"; }
const char* IrcCommand::ERR_INVALIDMODEVALUE::what() const throw() { return " :is invalid value to mode"; }

//KICK
const char* IrcCommand::ERR_NOPRIVILEGES::what() const throw() { return " :Permission Denied- You're not an IRC operator"; }
