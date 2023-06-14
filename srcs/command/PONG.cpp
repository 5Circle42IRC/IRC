#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::PONG(){
    _db->findClientByFd(_clientFd)->setNickname(getArgs()[0]);
}