#include "../include/IrcCommand.hpp"

IrcCommand::IrcCommand(IrcDB *db): _db(db) {
	_commandNames.push_back("INVITE");
	_commandNames.push_back("JOIN");
	_commandNames.push_back("NICK");
	_commandNames.push_back("PART");
	_commandNames.push_back("PONG");
	_commandNames.push_back("PRIVMSG");
	_commandNames.push_back("TOPIC");
	_commandNames.push_back("USER");
	_commandNames.push_back("MODE");
	_commandPointers[0] = &IrcCommand::INVITE;
	_commandPointers[1] = &IrcCommand::JOIN;
	_commandPointers[2] = &IrcCommand::NICK;
	_commandPointers[3] = &IrcCommand::PART;
	_commandPointers[4] = &IrcCommand::PONG;
	_commandPointers[5] = &IrcCommand::PRIVMSG;
	_commandPointers[6] = &IrcCommand::TOPIC;
	_commandPointers[7] = &IrcCommand::USER;
	_commandPointers[8] = &IrcCommand::MODE;
}
IrcCommand::IrcCommand(IrcDB *db, int clientFd): _db(db), _clientFd(clientFd) {
	_commandNames.push_back("INVITE");
	_commandNames.push_back("JOIN");
	_commandNames.push_back("NICK");
	_commandNames.push_back("PART");
	_commandNames.push_back("PONG");
	_commandNames.push_back("PRIVMSG");
	_commandNames.push_back("TOPIC");
	_commandNames.push_back("USER");
	_commandNames.push_back("MODE");
	_commandPointers[0] = &IrcCommand::INVITE;
	_commandPointers[1] = &IrcCommand::JOIN;
	_commandPointers[2] = &IrcCommand::NICK;
	_commandPointers[3] = &IrcCommand::PART;
	_commandPointers[4] = &IrcCommand::PONG;
	_commandPointers[5] = &IrcCommand::PRIVMSG;
	_commandPointers[6] = &IrcCommand::TOPIC;
	_commandPointers[7] = &IrcCommand::USER;
	_commandPointers[8] = &IrcCommand::MODE;
}
IrcCommand::~IrcCommand(){}


void IrcCommand::checkRunCMD(){
	int index = 0;
	for (std::vector<std::string>::iterator it = _commandNames.begin();
			it != _commandNames.end(); it++) {
		if (*it == _command){
			(this->*_commandPointers[index])();
			std::cout << "here" << "inside 2" <<std::endl;
			return ;
		}
		index++;
	}
	throw ERR_INVALID_COMMAND();

}

void IrcCommand::parsing(std::string message){
	int		start;
	int		end;
	std::string	delim = " ,\t\n\v\f\r";

	if (message.size() > 512)
		throw ERR_OUT_OF_BOUND_MESSAGE();
	_args.clear();
	message.erase(0, message.find_first_not_of(delim));
	std::cout << "message after whitespace: " << message << std::endl;
	for (end = message.find_first_of(delim); end != -1; end = message.find_first_of(delim)){
		_args.push_back(message.substr(0, end));
		message.erase(start, end + 1);
	}
	_args.push_back(message);
	_command = _args[0];
	std::cout << "command: " << _command << std::endl;
	std::cout << "args:" << _args[0] << "abd" << std::endl;
	_args.pop_front();
	checkRunCMD();
}

std::deque<std::string>& IrcCommand::getArgs(){ return _args; }
std::string IrcCommand::getCommand(){ return _command; }
IrcCommand& IrcCommand::setClientFd(int clientFd){ _clientFd = clientFd; return *this; }

//에러코드 결정해서 what의 내용은 에러코드를 반환해주도록 수정!
const char* IrcCommand::ERR_INVALID_PASSWORD::what() const throw() { return "ERR_INVALIDPASSWORD"; }
const char* IrcCommand::ERR_USER_ON_CHANNEL::what() const throw() { return "ERR_USERONCHANNEL"; }
const char* IrcCommand::ERR_INVALID_ARGUMENT::what() const throw() { return "ERR_INVALIDARGUMENT";}
const char* IrcCommand::ERR_INVALID_COMMAND::what() const throw() { return "ERR_INVALIDCOMMAND";}
const char* IrcCommand::ERR_OUT_OF_BOUND_MESSAGE::what() const throw() { return "ERR_OUT_OF_BOUND_MESSAGE"; }