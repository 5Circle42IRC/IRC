#include "../include/IrcCommand.hpp"

IrcCommand::IrcCommand(IrcDB *db): _db(db) {}
IrcCommand::~IrcCommand(){}

const	std::vector<std::string>& IrcCommand::getArgs() const{} // 삭제예정