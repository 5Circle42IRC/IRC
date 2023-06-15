#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

int IrcCommand::checkValidNICK(std::deque<std::string> args, IrcDB *_db)
{
    std::string nick = args[0]; 
    std::cout << "cc1    oldnick : <" << _db->findClientByFd(_clientFd)->getNickname() << ">   nick : <" << nick << ">" << std::endl;
    if (nick.size() > 9)
    {
        std::cout << "NICK size have to be lower than 9" << std::endl;
        /*
        ERR_ERRONEUSNICKNAME (432) 
        "<client> <nick> :Erroneus nickname"
        */        
        std::cout << "cc2" << std::endl;
        throw std::exception();
    }
    std::cout << "kk1" << std::endl;
    if (_db->findClientByFd(_clientFd)->getNickname() == nick)
    {
        std::cout << "Already same nick exists." << std::endl;
        std::cout << "nick <" << nick << ">  " << "ClientFd : <" << _db->findClientByName(nick)->getFd() << ">" << std::endl;
        std::cout << "cc3" << std::endl;
        throw std::exception();        
        /*
            ERR_NICKNAMEINUSE (433) 
            "<client> <nick> :Nickname is already in use"       
        */
    } 
    std::cout << "cc4" << std::endl;
    return 0;
}
void IrcCommand::NICK(){
    
        IrcClient* client = _db->findClientByFd(_clientFd);
        if (getArgs().size() != 1)
        {
            std::cout << "NICK args size is not 1" << std::endl;
            throw std::exception();        
            /*
            ERR_NONICKNAMEGIVEN (431) 
            "<client> :No nickname given"
            */            
        }
        else
        {
            std::cout << "c1" << std::endl;
            std::string oldNick = client->getNickname();
            std::cout << "c2  <" << oldNick << ">" << std::endl;
            checkValidNICK(getArgs(), _db);
            std::cout << "c3" << std::endl;
            _db->findClientByFd(_clientFd)->setNickname(getArgs()[0]);
            /*
            :WiZ NICK Kilroy 
            */
            client->addBackBuffer(":" + oldNick + " NICK " + client->getNickname() + "\r\n");
            
        }    
}
