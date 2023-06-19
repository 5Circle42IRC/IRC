#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

int IrcCommand::checkValidNICK(std::deque<std::string> args, IrcDB *_db)
{
    std::string newNick = args[0]; 
    std::string oldNick = _db->findClientByFd(_clientFd)->getNickname();
    if (newNick.size() > 9)
    {
        throw ERR_ERRONEUSNICKNAME();  
    }
    if (_db->findClientByFd(_clientFd)->getNickname() == newNick)
    {
        throw ERR_NICKNAMEINUSE();
    } 
    return 0;
}

void IrcCommand::NICK(){
    
        IrcClient* client = _db->findClientByFd(_clientFd);
        if (getArgs().size() != 1)
        {
            throw ERR_NONICKNAMEGIVEN();               
        }
        else
        {
            std::string oldNick = client->getNickname();
            
            checkValidNICK(getArgs(), _db);
            _db->findClientByFd(_clientFd)->setNickname(getArgs()[0]);
            
            //:WiZ NICK Kilroy 
            client->addBackBuffer(":" + oldNick + " NICK " + client->getNickname() + "\r\n");
            client->setPasswordFlag(true);
        }    
}
