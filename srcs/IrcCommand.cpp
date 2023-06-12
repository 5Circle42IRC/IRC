#include "../include/IrcCommand.hpp"

IrcCommand::IrcCommand(IrcDB *db): _db(db) {}
IrcCommand::~IrcCommand(){}

void parsing(std::string message){
	// size_t		start;
	// size_t		end;
	// std::string	delim = " ";

	// end = message.find(delim);
	// std::cout << end << std::endl;
	// start = 0;
	// for (end; end != -1; end = message.find(delim)){
	// 	_args.push_back(message.substr(start, end));
	// 	message.erase(start, end + 1);
	// 	start = end + 1;
	// } 
	// _args.push_back(message);
	// _command = _args[0];
}

// void Parser::parse(char *ori){
// 	std::string	comm = ori;
// 	size_t		start;
// 	size_t		end;
// 	std::string	delim = " ";

// 	end = comm.find(delim);
// 	std::cout << end << std::endl;
// 	start = 0;
// 	for (end; end != -1; end = comm.find(delim)){
// 		command.push_back(comm.substr(start, end));
// 		comm.erase(start, end + 1);
// 		start = end + 1;
// 	}
// 	command.push_back(comm);
// 	checkValidCommand(command);
// }