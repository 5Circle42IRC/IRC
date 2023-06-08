#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "../InterfaceClass/ICommand.hpp"

class IrcCommand: public ICommand {
	public :
		IrcCommand();
		virtual ~IrcCommand();

		virtual void	foreach(std::map<int, IClient&>& client) = 0;
		virtual void	foreach(std::map<std::string, IChannel&> channel) = 0;
		virtual void	execute(int ClientFd, std::vector<std::string>args) = 0;


		virtual const	std::vector<std::string>& getArgs() const = 0;
		virtual void	setArgs() = 0;

};

#endif;
