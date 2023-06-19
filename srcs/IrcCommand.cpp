#include "../include/IrcCommand.hpp"
#include "../include/IrcClient.hpp"

IrcCommand::IrcCommand(IrcDB *db): _db(db) {
	_commandList["INVITE"] = &IrcCommand::INVITE;
	_commandList["JOIN"] = &IrcCommand::JOIN;
	_commandList["NICK"] = &IrcCommand::NICK;
	_commandList["PART"] = &IrcCommand::PART;
	_commandList["PONG"] = &IrcCommand::PING;
	_commandList["PRIVMSG"] = &IrcCommand::PRIVMSG;
	_commandList["TOPIC"] = &IrcCommand::TOPIC;
	_commandList["USER"] = &IrcCommand::USER;
	_commandList["MODE"] = &IrcCommand::MODE;
	_commandList["DISPLAY"] = &IrcCommand::DISPLAY;
	_commandList["PASS"] = &IrcCommand::PASS;
	_commandList["KICK"] = &IrcCommand::KICK;	
	_commandNames.push_back("INVITE");
	_commandNames.push_back("JOIN");
	_commandNames.push_back("NICK");
	_commandNames.push_back("PART");
	_commandNames.push_back("PING");
	_commandNames.push_back("PRIVMSG");
	_commandNames.push_back("TOPIC");
	_commandNames.push_back("USER");
	_commandNames.push_back("MODE");
	_commandNames.push_back("DISPLAY");
	_commandNames.push_back("PASS");
	_commandNames.push_back("KICK");
	_commandPointers[0] = &IrcCommand::INVITE;
	_commandPointers[1] = &IrcCommand::JOIN;
	_commandPointers[2] = &IrcCommand::NICK;
	_commandPointers[3] = &IrcCommand::PART;
	_commandPointers[4] = &IrcCommand::PING;
	_commandPointers[5] = &IrcCommand::PRIVMSG;
	_commandPointers[6] = &IrcCommand::TOPIC;
	_commandPointers[7] = &IrcCommand::USER;
	_commandPointers[8] = &IrcCommand::MODE;
	_commandPointers[9] = &IrcCommand::DISPLAY;
	_commandPointers[10] = &IrcCommand::PASS;
	_commandPointers[11] = &IrcCommand::KICK;
}
IrcCommand::IrcCommand(IrcDB *db, int clientFd): _db(db), _clientFd(clientFd) {
	_commandList["INVITE"] = &IrcCommand::INVITE;
	_commandList["JOIN"] = &IrcCommand::JOIN;
	_commandList["NICK"] = &IrcCommand::NICK;
	_commandList["PART"] = &IrcCommand::PART;
	_commandList["PONG"] = &IrcCommand::PING;
	_commandList["PRIVMSG"] = &IrcCommand::PRIVMSG;
	_commandList["TOPIC"] = &IrcCommand::TOPIC;
	_commandList["USER"] = &IrcCommand::USER;
	_commandList["MODE"] = &IrcCommand::MODE;
	_commandList["DISPLAY"] = &IrcCommand::DISPLAY;
	_commandList["PASS"] = &IrcCommand::PASS;
	_commandList["KICK"] = &IrcCommand::KICK;	
	_commandNames.push_back("INVITE");
	_commandNames.push_back("JOIN");
	_commandNames.push_back("NICK");
	_commandNames.push_back("PART");
	_commandNames.push_back("PING");
	_commandNames.push_back("PRIVMSG");
	_commandNames.push_back("TOPIC");
	_commandNames.push_back("USER");
	_commandNames.push_back("MODE");
	_commandNames.push_back("DISPLAY");
	_commandNames.push_back("PASS");
	_commandNames.push_back("KICK");
	_commandPointers[0] = &IrcCommand::INVITE;
	_commandPointers[1] = &IrcCommand::JOIN;
	_commandPointers[2] = &IrcCommand::NICK;
	_commandPointers[3] = &IrcCommand::PART;
	_commandPointers[4] = &IrcCommand::PING;
	_commandPointers[5] = &IrcCommand::PRIVMSG;
	_commandPointers[6] = &IrcCommand::TOPIC;
	_commandPointers[7] = &IrcCommand::USER;
	_commandPointers[8] = &IrcCommand::MODE;
	_commandPointers[9] = &IrcCommand::DISPLAY;
	_commandPointers[10] = &IrcCommand::PASS;
	_commandPointers[11] = &IrcCommand::KICK;
}
IrcCommand::~IrcCommand(){}


void IrcCommand::checkRunCMD(){
	int index = 0;
	// std::map<std::string, commandPtrArr>
	for (std::vector<std::string>::iterator it = _commandNames.begin();
			it != _commandNames.end(); it++) {
		if (*it == _command){
			(this->*_commandPointers[index])();
			return ;
		}
		index++;
	}
	_db->findClientByFd(_clientFd)->addBackBuffer("ERR_INVALID_COMMAND\n");
}

void IrcCommand::parsing(std::string message){
	int		end;
	std::vector<std::string> multiCmd;
	std::string	delim = " ,\t\v\f";
	std::string endl = "\r\n";

	if (message.size() > 512)
		throw ERR_OUT_OF_BOUND_MESSAGE();
	message.erase(0, message.find_first_not_of(delim));
	// 다중메세지 개행, 캐리지리턴 기준으로 나누기 (동작 확인)
	for (end = message.find_first_of(endl); message.size() != 0 && end != -1; end = message.find_first_of(endl)){
		multiCmd.push_back(message.substr(0, end));
		message.erase(0, end + 1);
		if (message[0] == '\n' || '\r')
			message.erase(0);
	}
	multiCmd.push_back(message);
	if (multiCmd.back().size() == 0)
		multiCmd.pop_back();
	// 메세지 건바이건으로 커맨드 실행
	for (std::vector<std::string>::iterator it = multiCmd.begin(); it != multiCmd.end(); it++){
		int i = 0;
		_args.clear();
		for (end = it->find_first_of(delim); end != -1; end = it->find_first_of(delim)){
			_args.push_back(it->substr(0, end));
			it->erase(0, end + 1);
			if (i == 0){
				_command = _args[0];
				if (_command == "JOIN" || _command == "KICK")
					delim = " \t\v\f";
			}
			if ((!_command.compare("PRIVMSG") || !_command.compare("TOPIC")) && i > 0)
				break;
			if ((!_command.compare("KICK")) && i == 2)
				break;
			i++;
		}
		if (*it != "")
			_args.push_back(*it);
		if (_args.size() == 1)
			_command = _args[0];
		_args.pop_front();
		try {
			checkRunCMD();
		} catch (std::exception &e){
			_db->findClientByFd(_clientFd)->addBackCarriageBuffer(e.what());
		}
	}
}

std::deque<std::string>& IrcCommand::getArgs(){ return _args; }
std::string IrcCommand::getCommand(){ return _command; }
IrcCommand& IrcCommand::setClientFd(int clientFd){ _clientFd = clientFd; return *this; }

//에러코드 결정해서 what의 내용은 에러코드를 반환해주도록 수정!
//JOIN
const char* IrcCommand::ERR_INVALID_PASSWORD::what() const throw() { return "ERR_INVALID_PASSWORD"; }
const char* IrcCommand::ERR_USER_ON_CHANNEL::what() const throw() { return "ERR_USER_ON_CHANNEL"; }
const char* IrcCommand::ERR_INVALID_ARGUMENT::what() const throw() { return "ERR_INVALID_ARGUMENT"; }
const char* IrcCommand::ERR_OUT_OF_LIMIT::what() const throw() { return "ERR_OUT_OF_LIMIT";}
const char* IrcCommand::ERR_INVITE_PERSON_ONLY::what() const throw() { return "ERR_INVITE_PERSON_ONLY";}

//PARSING
const char* IrcCommand::ERR_INVALID_COMMAND::what() const throw() { return "ERR_INVALID_COMMAND";}
const char* IrcCommand::ERR_OUT_OF_BOUND_MESSAGE::what() const throw() { return "ERR_OUT_OF_BOUND_MESSAGE"; }
const char* IrcCommand::ERR_INVALID_NAME_OF_CHANNEL::what() const throw() { return "ERR_INVALID_NAME_OF_CHANNEL";}
const char* IrcCommand::ERR_INVALID_CHAR_IN_NAME::what() const throw() { return "ERR_INVALID_CHAR_IN_NAME"; }


//NICK
const char* IrcCommand::ERR_NICKNAMEINUSE::what() const throw() { return "ERR_NICKNAMEINUSE"; }
const char* IrcCommand::ERR_NONICKNAMEGIVEN::what() const throw() { return "ERR_NONICKNAMEGIVEN"; }
const char* IrcCommand::ERR_ERRONEUSNICKNAME::what() const throw() { return "ERR_ERRONEUSNICKNAME"; }
//PART
const char* IrcCommand::ERR_NOTONCHANNEL::what() const throw() { return "ERR_NOTONCHANNEL"; }
//TOPIC
const char* IrcCommand::ERR_NEEDMOREPARAMS::what() const throw() { return "ERR_NEEDMOREPARAMS"; }
const char* IrcCommand::ERR_CHANOPRIVSNEEDED::what() const throw() { return "ERR_CHANOPRIVSNEEDED"; }

//MODE
const char* IrcCommand::ERR_UNKNOWNMODE::what() const throw() { return "ERR_UNKNOWNMODE"; }

//KICK
const char* IrcCommand::ERR_NOT_OPERATOR::what() const throw() { return "ERR_NOT_OPERATOR"; }

