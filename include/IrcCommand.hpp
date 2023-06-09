#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "./IrcDB.hpp"
# include <vector>

class IrcCommand: protected IrcDB {
	public :
		IrcCommand();
		virtual ~IrcCommand();

		void	foreach(std::map<int, IrcClient&>& client);
		void	foreach(std::map<std::string, IrcChannel&> channel);

		const	std::vector<std::string>& getArgs() const;

		void KICK(int clientFd);
		void INVITE(int clientFd);

		void JOIN(int clientFd);
		void NICK(int clientFd);
		void PASS(int clientFd);
		void NAME(int clientFd);
		void PRIVMSG(int clientFd);

		void TOPIC(int clientFd);
		void MODE(int clientFd);

		void PART(int clientFd);
		void PONG(int clientFd);
		void USER(int clientFd);

	protected:
		std::vector<std::string>  _args;

	private:
		class InvalidArguments: public std::exception {
			virtual const char *what() const throw();
		};

};

#endif