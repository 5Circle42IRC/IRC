#include "../include/IrcCommand.hpp"

IrcCommand::IrcCommand(IrcDB *db): _db(db) {}
IrcCommand::~IrcCommand(){}

void IrcCommand::parsing(std::string message){
	size_t		start;
	size_t		end;
	std::string	delim = " ";

	_args.clear();
	end = message.find(delim);
	start = 0;
	for (end; end != -1; end = message.find(delim)){
		_args.push_back(message.substr(start, end));
		message.erase(start, end + 1);
		start = end + 1;
	} 
	_args.push_back(message);
	_command = _args[0];
	_args.pop_front();
}

std::deque<std::string>& IrcCommand::getArgs(){ return _args; }
std::string IrcCommand::getCommand(){return _command;}

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