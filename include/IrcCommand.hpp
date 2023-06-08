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

<<<<<<< HEAD
#endif;
=======
#endif;
>>>>>>> 37ddfd7eb53da24f168babb4e1f4ca82c1175692
