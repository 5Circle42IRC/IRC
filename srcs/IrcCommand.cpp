#include "../include/IrcCommand.hpp"

IrcCommand::IrcCommand(IrcDB *db): _db(db) {}
IrcCommand::~IrcCommand(){}

void IrcCommand::parsing(std::string message){
	int		start;
	int		end;
	std::string	delim = " ,\t\n\v\f\r";

	_args.clear();
	for (end = message.find_first_of(delim); end != -1; end = message.find_first_of(delim)){
		_args.push_back(message.substr(0, end));
		message.erase(start, end + 1);
		if (_args[0] == "PRIVMSG"){
			_args[1] = message;
			break;
		}
			
	}
	_args.push_back(message);
	_command = _args[0];
	_args.pop_front();
}

std::deque<std::string>& IrcCommand::getArgs(){ return _args; }
std::string IrcCommand::getCommand(){ return _command; }

const char* IrcCommand::ERR_INVALIDPASSWORD::what() const throw() { return "ERR_INVALIDPASSWORD"; }
const char* IrcCommand::ERR_USERONCHANNEL::what() const throw() { return "ERR_USERONCHANNEL"; }