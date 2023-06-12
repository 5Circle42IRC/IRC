#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::USER(int clientfd){
    _db->findClientByFd(clientfd)->setNickname(getArgs()[0]);
}