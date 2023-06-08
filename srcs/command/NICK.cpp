#include "../../include/IrcCommand.hpp"


//맞춰서 추가해야 함. jwee가 수정할거임???

NICK::NICK(){}
NICK::~NICK(){}

void execute(Vector<std::string> args, Client& target){
	target.setNickname(args[0]);
}