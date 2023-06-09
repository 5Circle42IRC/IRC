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

		void KICK();
		void INVITE();

		void JOIN(int clientFd);
		void NICK();
		void PASS();
		void NAME();
		void PRIVMSG();

		void TOPIC();
		void MODE();

		void PART();
		void PONG();

	protected:
		std::vector<std::string>  _args;

	private:
		class InvalidArguments: public std::exception {
			virtual const char *what() const throw();
		};

};

#endif
