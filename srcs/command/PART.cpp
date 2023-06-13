#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"

void IrcCommand::PART(int clientfd){
    std::string chname = getArgs()[0];
    IrcChannel* ch = _db->findChannel(chname);
    ch->deleteUser(clientfd);
}