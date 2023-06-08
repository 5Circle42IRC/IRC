#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "../InterfaceClass/ICommand.hpp"

class IrcCommand: public ICommand {
	public :
		IrcCommand();
		virtual ~IrcCommand();

		void	foreach(T);
		void	execute(vector<std::string>args);

		const	vector<std::string>& getArgs() const;
		void	setArgs();
};

#endif;
