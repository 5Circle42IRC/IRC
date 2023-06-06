#include "../include/IrcCommand.hpp"

ACommand::ACommand(){}
ACommand::~ACommand(){}


void Parser::run() {}
void Parser::checkValidCommand() {
	std::string commands[10] = {"NAME", "PASS", "USER", "INVITE", "JOIN", "TOPIC", "MODE", "KICK", "PART", "PONG"};
	for (int i = 0; command[0] != commands[i] && i < 10; i++){
		if (i > 9)
			throw std::exception();
	}
}

void Parser::parse(const char *ori){
	std::string	comm = ori;
	int			start;
	int			end;
	std::string	delim = " ";

	start = 0;
	while ((end = comm.find(delim)) != -1)
	{
		command.push_back(comm.substr(start, end));
		comm.erase(start, end + 1);
		start = end + 1;

	}
	command.push_back(comm);
	std::cout << command[0] << "***" << command[1] << std::endl;
	checkValidCommand();
}

int main() {
	Parser a;
	const char *str = "KICK jwee";

	a.parse(str);
}