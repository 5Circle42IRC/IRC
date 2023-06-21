#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

int IrcCommand::checkValidNICK(std::deque<std::string> args, IrcDB *_db)
{
    std::string newNick = args[0]; 
    std::string oldNick = _db->findClientByFd(_clientFd)->getNickname();
    IrcClient* client = _db->findClientByFd(_clientFd);
    if (newNick.size() > 9)
    {
        client->addBackBuffer(":localhost 432 " + client->getNickname() + " " + newNick);
        throw ERR_ERRONEUSNICKNAME();  
    }

    std::map<int, IrcClient*>clients = _db->getAllClients();
    std::map<int, IrcClient*>::iterator it;
    for (it = clients.begin(); it != clients.end(); it ++)
    {
        
        if (it->second->getNickname() == newNick)
        {        
            client->addBackBuffer(":localhost 433 " + client->getNickname() + " " + newNick);   
            throw ERR_NICKNAMEINUSE();
        }         
    }

    return 0;
}

void IrcCommand::NICK(){
        std::cout << "start NICK" << std::endl;
        IrcClient* client = _db->findClientByFd(_clientFd);
        if (getArgs().size() != 1)
        {
            std::cout << "no nick given" << std::endl;
            //":localhost 431 " + client + " :There is no nickname.\r\n"
            client->addBackBuffer(":localhost 431 " + client->getNickname() + " ");            
            throw ERR_NONICKNAMEGIVEN();               
        }
        else
        {
            std::cout << "n1" << std::endl;
            std::string oldNick = client->getNickname();
            std::cout << "n2" << std::endl;
            checkValidNICK(getArgs(), _db);
            std::cout << "n3" << std::endl;
            _db->findClientByFd(_clientFd)->setNickname(getArgs()[0]);
            std::cout << "n4" << std::endl;
            //:WiZ NICK Kilroy 
            client->addBackBuffer(":" + oldNick + " NICK :" + client->getNickname() + "\r\n");
            client->setPasswordFlag(true);
        }    
}
