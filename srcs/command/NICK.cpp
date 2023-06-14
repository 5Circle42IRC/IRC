#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

static int checkValidNICK(std::deque<std::string> args, IrcDB *_db)
{
    std::string nick = args[0]; 
    if (nick.size() > 9)
    {
        std::cout << "NICK size have to be lower than 9" << std::endl;
        throw std::exception();
    }
    if (_db->findClientByName(nick)->getNickname() == nick)
    {
        std::cout << "Already same nick exists." << std::endl;
        std::cout << "nick <" << nick << ">  " << "ClientFd : <" << _db->findClientByName(nick)->getFd() << ">" << std::endl;
        throw std::exception();        
    } 
    return 1;
}
void IrcCommand::NICK(){
    
        if (getArgs().size() != 1)
            std::cout << "NICK args size is not 1" << std::endl;
        else
        {
            if (checkValidNICK(getArgs(), _db) == 1 )
                _db->findClientByFd(_clientFd)->setNickname(getArgs()[0]);
        }    
}
