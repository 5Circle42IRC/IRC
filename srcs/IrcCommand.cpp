#include "../include/IrcCommand.hpp"

ACommand::ACommand(){}
ACommand::~Acommand(){}

static void checkValidCommand(std::vector<std::string> command) {
	std::string commands[] = {"NAME", "PASS", "USER", "INVITE", "JOIN", "TOPIC", "MODE", "KICK", "PART", "PONG"};
	for (int i = 0; comm[0] != commands[i] && i < 10; i++);
	if (i > 9)
		throw new Error();
}

void ACommand::parser(char *str){
	size_t						start;
	size_t						end;
	std::string					delim = " ";

	command = str;
	end = ori.find(delim)
	start = 0;
	for (end; end != std::string::npos; end = ori.find(delim)){
		command.push_back(ori.substr(start, end));
		start = end + 1;
	}
	command.push_back(ori.substr(start, ori.length()));
	checkValidCommand(command);
}