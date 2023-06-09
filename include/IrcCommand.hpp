#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "./IrcDB.hpp"
# include <vector>

class IrcCommand: protected IrcDB {
	public :
		IrcCommand();
		virtual ~IrcCommand();

		virtual void	foreach(std::map<int, IrcClient&>& client) = 0;
		virtual void	foreach(std::map<std::string, IrcChannel&> channel) = 0;
		virtual void	execute(int ClientFd, std::vector<std::string>args) = 0;


		virtual const	std::vector<std::string>& getArgs() const;

	private:
		std::vector<std::string>  _args;

};

#endif;
