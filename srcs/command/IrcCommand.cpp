#include "../../include/IrcCommand.hpp"

IrcCommand::IrcCommand(){}
IrcCommand::~IrcCommand(){}

const	std::vector<std::string>& IrcCommand::getArgs() const{ return _args; }