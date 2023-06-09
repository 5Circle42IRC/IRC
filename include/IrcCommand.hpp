#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "./IrcDB.hpp"
# include <vector>

class IrcCommand: protected IrcDB {
	public :
		IrcCommand();
		virtual ~IrcCommand();

		virtual void	foreach(std::map<int, IrcClient&>& client);
		virtual void	foreach(std::map<std::string, IrcChannel&> channel);
		virtual void	execute(int ClientFd, std::vector<std::string>args);


		virtual const	std::vector<std::string>& getArgs() const;

	protected:
		std::vector<std::string>  _args;

};

#endif
