#include "../../include/IrcCommand.hpp"

<<<<<<< HEAD
NICK::NICK(){}
NICK::~NICK(){}

void NICK::execute(vector<std::string> args){

	if (args.size() != 1)
		throw std::exception(); //
	_nickname = args[0];
}
=======

//맞춰서 추가해야 함. jwee가 수정할거임???

NICK::NICK(){}
NICK::~NICK(){}
>>>>>>> 37ddfd7eb53da24f168babb4e1f4ca82c1175692

void execute(Vector<std::string> args, Client& target){
	target.setNickname(args[0]);
}