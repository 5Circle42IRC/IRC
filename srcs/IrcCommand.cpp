#include "../include/IrcCommand.hpp"

ACommand::ACommand(){}
ACommand::~Acommand(){}

static void checkValidCommand(std::vector<std::string> command) {
	std::string commands[] = {"NAME", "PASS", "USER", "INVITE", "JOIN", "TOPIC", "MODE", "KICK", "PART", "PONG"};
	for (int i = 0; comm[0] != commands[i] && i < 10; i++);
	if (i > 9)
		throw new Error();
}

void Parser::parse(char *ori){
	std::string	comm = ori;
	size_t		start;
	size_t		end;
	std::string	delim = " ";

	end = comm.find(delim);
	std::cout << end << std::endl;
	start = 0;
	for (end; end != -1; end = comm.find(delim)){
		command.push_back(comm.substr(start, end));
		comm.erase(start, end + 1);
		start = end + 1;
	}
	command.push_back(comm);
	checkValidCommand(command);
}