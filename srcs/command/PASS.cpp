#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

int IrcCommand::checkValidPASS(std::deque<std::string> args, IrcDB *_db)
{

    return 0;
}

void IrcCommand::PASS(){
    
        IrcClient* client = _db->findClientByFd(_clientFd);

        if (getArgs().size() != 1)
        {
            throw ERR_NONICKNAMEGIVEN();               
        }
        else
        {
            
            client->addBackBuffer(":" + client->getNickname() + " PASS " + _args[1] + "\r\n");
        }    
}