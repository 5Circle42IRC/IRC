#ifndef IRCCOMMAND_H
# define IRCCOMMAND_H

# include "Irc.hpp"

class ACommand {
	public:
		ACommand();
		virtual ~ACommand();

		virtual void run() = 0;
	protected:
		std::vector<std::string> command;
};

class Parser : public ACommand {
	public:
		void run();
		void parse(const char *ori);
		void checkValidCommand();
		
};

#endif