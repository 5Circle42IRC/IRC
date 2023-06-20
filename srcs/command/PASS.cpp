#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::PASS(){
    
        IrcClient* client = _db->findClientByFd(_clientFd);

        if (getArgs().size() != 1)
        {
            throw ERR_NEED_MORE_PARAMS();               
        }
        else
        {
            client->addBackBuffer(":" + client->getNickname() + " PASS " + _args[0] + "\r\n");
        }    
}