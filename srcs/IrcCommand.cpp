#include "../include/IrcCommand.hpp"

IrcCommand::IrcCommand(IrcDB *db, int clientFd): _db(db), _clientFd(clientFd) {}
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

//에러코드 결정해서 what의 내용은 에러코드를 반환해주도록 수정!
const char* IrcCommand::ERR_INVALID_PASSWORD::what() const throw() { return "ERR_INVALIDPASSWORD"; }
const char* IrcCommand::ERR_USER_ON_CHANNEL::what() const throw() { return "ERR_USERONCHANNEL"; }
const char* IrcCommand::ERR_INVALID_ARGUMENT::what() const throw() { return "ERR_INVALIDARGUMENT";}