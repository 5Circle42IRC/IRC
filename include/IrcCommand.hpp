#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "./IrcDB.hpp"
# include <deque>

//_clientFd 또는 _clientPtr을 멤버변수로 추가하는 건에 대하여...

class IrcCommand {
	public :
		IrcCommand(IrcDB *db, int clientFd);
		~IrcCommand();

		void parsing(std::string message);
		// void KICK(int clientFd);
		// void INVITE(int clientFd);

		void JOIN();
		void NICK();
		// void PASS();
		// void NAME();
		void PRIVMSG();

		void TOPIC(int clientFd);
		void MODE(int clientFd);

		void PART();
		void PONG();
		void USER();
		std::deque<std::string>& getArgs();
		std::string getCommand();



	private:
		IrcCommand();

		IrcDB* 						_db;
		std::deque<std::string>		_args;
		std::string 				_command;
		int							_clientFd;

		void joinChannel(std::map<std::string, std::string>& keypair);

		class ERR_INVALID_ARGUMENT: public std::exception {
			virtual const char *what() const throw();
		};
		
		class ERR_INVALID_PASSWORD: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_USER_ON_CHANNEL: public std::exception {
			virtual const char *what() const throw();
		};
};

#endif