#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "./IrcDB.hpp"
# include <deque>

class IrcCommand {
	public :
		IrcCommand(IrcDB *db);
		~IrcCommand();

		const	std::vector<std::string>& getArgs() const; //삭제 예정

		void parsing(std::string message);
		// void KICK(int clientFd);
		// void INVITE(int clientFd);

		// void JOIN(int clientFd);
		void NICK(int clientFd);
		// void PASS(int clientFd);
		// void NAME(int clientFd);
		// void PRIVMSG(int clientFd);

		// void TOPIC(int clientFd);
		// void MODE(int clientFd);

		// void PART(int clientFd);
		// void PONG(int clientFd);
		// void USER(int clientFd);

	private:
		IrcCommand();

		IrcDB* 						_db;
		std::deque<std::string>		_args;
		std::string 				_command;

		class InvalidArguments: public std::exception {
			virtual const char *what() const throw();
		};

};

#endif