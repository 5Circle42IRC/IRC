/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:45:45 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/21 18:52:36 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "./IrcDB.hpp"
# include <deque>
# include <vector>

class IrcCommand {
	public :
		/*db만 받는 생성자는 테스트 이후 삭제해도 됨.*/
		IrcCommand(IrcDB *db, int clientFd);
		~IrcCommand();

		void parsing(std::string message);
		void INVITE();

		void JOIN();
		void NICK();
		void PRIVMSG();

		void TOPIC();
		void MODE();

		void PART();
		void PING();
		void USER();
		void PASS();
		void DISPLAY();
		void KICK();

		IrcCommand& setClientFd(int clientFd);
		std::deque<std::string>& getArgs();
		std::string getCommand();



	private:
		IrcCommand();

		IrcDB* 						_db;
		std::deque<std::string>		_args;
		std::string 				_command;
		int							_clientFd;

		typedef void (IrcCommand::*commandPtr)();
		std::map<std::string, commandPtr> _commandList;

		void joinChannel(std::string name, std::string key);
		int checkValidNICK(std::deque<std::string> args, IrcDB *_db);
		void checkRunCMD();
		void kickUser(std::string channel, std::string name, std::string comment);
		void makeBufferString(IrcChannel *channel, IrcClient *client);

		class ERR_BADCHANNELKEY: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_USERONCHANNEL: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_UNKNOWNCOMMAND: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_OUTOFBOUNDMESSAGE: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_BADCHANNELNAME: public std::exception {
			virtual const char *what() const throw();
		};
        class ERR_NICKNAMEINUSE: public std::exception {
            virtual const char *what() const throw();
        };
        class ERR_NONICKNAMEGIVEN : public std::exception {
            virtual const char *what() const throw();
        };
        class ERR_ERRONEUSNICKNAME : public std::exception {
            virtual const char *what() const throw();
        };
        //PART
        class ERR_NOTONCHANNEL : public std::exception {
            virtual const char *what() const throw();
        };
        //TOPIC
        class ERR_NEEDMOREPARAMS : public std::exception {
            virtual const char *what() const throw();
        };
        class ERR_CHANOPRIVSNEEDED : public std::exception {
            virtual const char *what() const throw();
        };
		class ERR_UNKNOWNMODE : public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_CHANNELISFULL : public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_INVITEONLYCHAN : public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_NOPRIVILEGES : public std::exception {
			virtual const char *what() const throw();
		};
};

#endif
