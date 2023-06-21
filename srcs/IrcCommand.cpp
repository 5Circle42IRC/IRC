#include "../include/IrcCommand.hpp"
#include "../include/IrcClient.hpp"

IrcCommand::IrcCommand(IrcDB *db, int clientFd): _db(db), _clientFd(clientFd) {
	_commandList["INVITE"] = &IrcCommand::INVITE;
	_commandList["JOIN"] = &IrcCommand::JOIN;
	_commandList["NICK"] = &IrcCommand::NICK;
	_commandList["PART"] = &IrcCommand::PART;
	_commandList["PING"] = &IrcCommand::PING;
	_commandList["PRIVMSG"] = &IrcCommand::PRIVMSG;
	_commandList["TOPIC"] = &IrcCommand::TOPIC;
	_commandList["USER"] = &IrcCommand::USER;
	_commandList["MODE"] = &IrcCommand::MODE;
	_commandList["DISPLAY"] = &IrcCommand::DISPLAY;
	_commandList["KICK"] = &IrcCommand::KICK;
	_commandList["PASS"] = &IrcCommand::PASS;
}
IrcCommand::~IrcCommand(){}

void IrcCommand::checkRunCMD(){
	int index = 0;
	std::map<std::string, commandPtr>::iterator commandIter;
	for (commandIter = _commandList.begin(); commandIter != _commandList.end(); commandIter++){
		if (commandIter->first == _command){
			commandPtr funcPtr = commandIter->second;
			(this->*funcPtr)();
			return ;
		}
	}
	_db->findClientByFd(_clientFd)->addBackCarriageBuffer("ERR_UNKNOWNCOMMAND\n");
}

void IrcCommand::parsing(std::string message){
	int		end;
	std::vector<std::string> multiCmd;
	std::string	delim = " ,\t\v\f";
	std::string endl = "\r\n";
	IrcClient *client = _db->findClientByFd(_clientFd);

	_args.clear();

	std::cout << "password flag : " << client->getPasswordFlag() << std::endl;
	if (client->getPasswordFlag() < 3){
		if (client->getPasswordFlag() == 0){
			if (message.substr(0, 5) == "PASS "){
				message.erase(0, 4);
				message.erase(0, message.find_first_not_of(delim));
				_command = "PASS";
				for (end = message.find_first_of(delim); end != -1; end = message.find_first_of(delim)){
					_args.push_back(message.substr(0, end));
					message.erase(0, end);
					message.erase(0, message.find_first_not_of(delim));
				}
				if (message.size() > 0)
					_args.push_back(message.substr(0, message.find_first_of(endl)));
				if (_args.size() != 1){
					client->addBackCarriageBuffer("input server password using PASS command");
					return ;
				}
				if (_args[0] != _db->getServPass()){
					client->addBackCarriageBuffer("input server password using PASS command");
					return ;
				}
				try {
					checkRunCMD();
				} catch (std::exception &e){
					client->addBackCarriageBuffer("input server password using PASS command");
					return ;
				}
				client->setPasswordFlag(1);
				client->addBackCarriageBuffer("input your nickname using NICK command");
				return ;
			}
			else {
				client->addBackCarriageBuffer("input server password using PASS command");
				return;
			}
		}
		if (client->getPasswordFlag() == 1){
			if (message.substr(0, 5) == "NICK "){
				message.erase(0, 4);
				message.erase(0, message.find_first_not_of(delim));
				_command = "NICK";
				for (end = message.find_first_of(delim); end != -1; end = message.find_first_of(delim)){
					_args.push_back(message.substr(0, end));
					message.erase(0, end);
					message.erase(0, message.find_first_not_of(delim));
				}
				if (message.size() > 0)
					_args.push_back(message.substr(0, message.find_first_of(endl)));
				try {
					checkRunCMD();
				} catch (std::exception &e){
					client->addBackCarriageBuffer("input your nickname using NICK command");
					return ;
				}
				client->setPasswordFlag(2);
				client->addBackCarriageBuffer("input your username using USER command");
				return ;
			}
			else {
				client->addBackCarriageBuffer("input your nickname using NICK command");
				return ;
			}
		}
		if (client->getPasswordFlag() == 2){
			if (message.substr(0, 5) == "USER "){
				message.erase(0, 5);
				_command = "USER";
				for (end = message.find_first_of(delim); end != -1; end = message.find_first_of(delim)){
					_args.push_back(message.substr(0, end));
					message.erase(0, end);
					message.erase(0, message.find_first_not_of(delim));
				}
				if (message.size() > 0)
					_args.push_back(message.substr(0, message.find_first_of(endl)));
				try {
					checkRunCMD();
				} catch (std::exception &e){
					client->addBackCarriageBuffer("input your username using USER command");
					return ;
				}
				client->setPasswordFlag(3);
				std::string test("hi  test   ");
				client->addBackCarriageBuffer("001 " + test + " :Welcome " + "dldldl" + " to irc ysungwon");
				return ;
			}
			else {
				client->addBackCarriageBuffer("input your username using USER command");
				return;
			}
		}
	}
	if (message.size() > 512)
		throw ERR_OUTOFBOUNDMESSAGE();
	message.erase(0, message.find_first_not_of(delim));
	// 다중메세지 개행, 캐리지리턴 기준으로 나누기 (동작 확인)
	for (end = message.find_first_of(endl); message.size() != 0 && end != -1; end = message.find_first_of(endl)){
		multiCmd.push_back(message.substr(0, end));
		message.erase(0, end + 1);
		if (message[0] == '\n' || '\r')
			message.erase(0);
	}
	multiCmd.push_back(message);
	// multiCmd 출력 확인용
	for (std::vector<std::string>::iterator mulit = multiCmd.begin(); mulit != multiCmd.end(); mulit++){
		std::cout << "&&&" <<  *mulit  << "&&&" << std::endl;
	}
	if (multiCmd.back().size() == 0)
		multiCmd.pop_back();
	// 메세지 건바이건으로 커맨드 실행
	for (std::vector<std::string>::iterator it = multiCmd.begin(); it != multiCmd.end(); it++){
		int i = 0;
		_args.clear();
		for (end = it->find_first_of(delim); end != -1; end = it->find_first_of(delim)){
			_args.push_back(it->substr(0, end));
			it->erase(0, end);
			it->erase(0, it->find_first_not_of(delim));
			if (i == 0){
				_command = _args[0];
				if (_command == "JOIN" || _command == "KICK" || _command == "PART")
					delim = " \t\v\f";
			}
			if ((!_command.compare("PRIVMSG") || !_command.compare("TOPIC")) && i > 0)
				break;
			if ((!_command.compare("KICK")) && i == 2)
				break;
			i++;
		}
		if ((*it).size() > 0)
			_args.push_back(*it);
		//파싱 확인 출력
		// for (std::deque<std::string>::iterator it2 = _args.begin(); it2 != _args.end(); it2++){
		// 	std::cout << "<" << *it2 << ">" << std::endl;
		// }
		if (_args.size() == 1)
			_command = _args[0];
		_args.pop_front();
		try {
			checkRunCMD();
		} catch (std::string name) {
			client->addBackCarriageBuffer("403 <" + name + "> :No such channel");
		} catch (char *name){
			client->addBackBuffer("401 <");
			client->addBackBuffer(name);
			client->addBackCarriageBuffer("> :No such nick/channel");
		}
		 catch (std::exception &e){
			client->addBackCarriageBuffer(e.what());
		}
	}
}

std::deque<std::string>& IrcCommand::getArgs(){ return _args; }
std::string IrcCommand::getCommand(){ return _command; }
IrcCommand& IrcCommand::setClientFd(int clientFd){ _clientFd = clientFd; return *this; }

//에러코드 결정해서 what의 내용은 에러코드를 반환해주도록 수정!
//JOIN
const char* IrcCommand::ERR_BADCHANNELKEY::what() const throw() { return " :cannot join channel (+k)"; }
const char* IrcCommand::ERR_USERONCHANNEL::what() const throw() { return " :is already on channel"; }
const char* IrcCommand::ERR_NEEDMOREPARAMS::what() const throw() { return " :Not enough parameters"; }
const char* IrcCommand::ERR_CHANNELISFULL::what() const throw() { return " :cannot join channel (+l)";}
const char* IrcCommand::ERR_INVITEONLYCHAN::what() const throw() { return " :cannot join channel (+i)";}

//PARSING
const char* IrcCommand::ERR_UNKNOWNCOMMAND::what() const throw() { return " :Unknown command";}
const char* IrcCommand::ERR_OUTOFBOUNDMESSAGE::what() const throw() { return " :command is too long"; }
const char* IrcCommand::ERR_BADCHANNELNAME::what() const throw() { return " :channel name is too long";}

//NICK
const char* IrcCommand::ERR_NICKNAMEINUSE::what() const throw() { return " :Nickname is already in use"; }
const char* IrcCommand::ERR_NONICKNAMEGIVEN::what() const throw() { return " :No nickname given"; }
const char* IrcCommand::ERR_ERRONEUSNICKNAME::what() const throw() { return " :Erroneus nickname"; }
//PART
const char* IrcCommand::ERR_NOTONCHANNEL::what() const throw() { return " :You're not on that channel"; }
//TOPIC
const char* IrcCommand::ERR_CHANOPRIVSNEEDED::what() const throw() { return " :You're not channel operator"; }

//MODE
const char* IrcCommand::ERR_UNKNOWNMODE::what() const throw() { return " :is unknwon mode char to me"; }

//KICK
const char* IrcCommand::ERR_NOPRIVILEGES::what() const throw() { return " :Permission Denied- You're not an IRC operator"; }
