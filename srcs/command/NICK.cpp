#include "../../include/IrcCommand.hpp"

NICK::NICK(){}
NICK::~NICK(){}

void NICK::execute(vector<std::string> args){

	if (args.size() != 1)
		throw std::exception(); //
	_nickname = args[0];
}

void execute(Vector<std::string> args, Client& target){
	target.setNickname(args[0]);
}