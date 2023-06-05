#ifndef IRCCOMMAND_H
# define IRCCOMMAND_H

class ACommand {
	public:
		ACommand();
		virtual ~ACommand();

		virtual void run() = 0;
		virtual void parser(char *str);
	private:
		std::vector<std::string> command;
};

#endif