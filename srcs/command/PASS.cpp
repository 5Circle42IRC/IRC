#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::PASS(){
    
        IrcClient* client = _db->findClientByFd(_clientFd);

        if (getArgs().size() != 1)
        {
            throw ERR_NEEDMOREPARAMS();               
        }
        else
        {
            client->setPassword(_args[0]);
            client->addBackBuffer(":" + client->getNickname() + " PASS " + _args[0] + "\r\n");
        }    
}