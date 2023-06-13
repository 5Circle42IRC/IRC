#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "./IrcDB.hpp"
# include <deque>

//_clientFd 또는 _clientPtr을 멤버변수로 추가하는 건에 대하여...

class IrcCommand {
	public :
		IrcCommand(IrcDB *db);
		~IrcCommand();

		void parsing(std::string message);
		// void KICK(int clientFd);
		// void INVITE(int clientFd);

		void JOIN(int clientFd);
		void NICK(int clientFd);
		// void PASS(int clientFd);
		// void NAME(int clientFd);
		void PRIVMSG(int clientFd);

		void TOPIC(int clientFd);
		// void MODE(int clientFd);

		void PART(int clientFd);
		void PONG(int clientFd);
		void USER(int clientFd);
		std::deque<std::string>& getArgs();
		std::string getCommand();



	private:
		IrcCommand();

		IrcDB* 						_db;
		std::deque<std::string>		_args;
		std::string 				_command;

		void joinChannel(std::map<std::string, std::string>& keypair, int clientFd);

		class InvalidArguments: public std::exception {
			virtual const char *what() const throw();
		};
		
		class ERR_INVALIDPASSWORD: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_USERONCHANNEL: public std::exception {
			virtual const char *what() const throw();
		};
};

#endif