#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

int IrcCommand::checkValidNICK(std::deque<std::string> args, IrcDB *_db)
{
    std::string newNick = args[0]; 
    std::string oldNick = _db->findClientByFd(_clientFd)->getNickname();
    if (newNick.size() > 9)
    {
        throw ERR_ERRONEUS_NICKNAME();  
    }
    if (_db->findClientByFd(_clientFd)->getNickname() == newNick)
    {
        _db->findClientByFd(_clientFd)->addBackBuffer("433 " + newNick + " " + newNick + " :Nickname is already in use");
        throw ERR_NICKNAME_IN_USE();
    } 
    return 0;
}

void IrcCommand::NICK(){
    
        IrcClient* client = _db->findClientByFd(_clientFd);
        if (getArgs().size() != 1)
        {
            throw ERR_NO_NICKNAME_GIVEN();               
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
