#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcDB.hpp"

void IrcCommand::DISPLAY(){
    
    IrcClient* client = _db->findClientByFd(_clientFd);
    std::cout << "------------Display -------" << std::endl;
    std::cout << "ClientFd : <" << client->getFd() << ">" << std::endl;
    std::cout << "Nick : <" << client->getNickname() << ">" << std::endl;
    std::cout << "UserName : <" << client->getUsername() << ">" << std::endl;
    std::cout << "HostName : <" << client->getHostname() << ">" << std::endl;
    std::cout << "ServerName : <" << client->getServername() << ">" << std::endl;
    std::cout << "RealName : <" << client->getRealname() << ">" << std::endl;
    std::cout << "getBuffer : <" << client->getBuffer()  << ">" << std::endl;
    
    

    std::map<std::string , IrcChannel*>  _channels = _db->_channels;
    std::cout << "/////all channel////" << std::endl;
    std::cout << "all channel size : <" << _channels.size() << ">" << std::endl;
    std::map<std::string , IrcChannel*>::iterator it;
    for (it = _channels.begin();
        it != _channels.end();
        it++)
        {
                std::cout << "clientFd : <" << _clientFd << ">" << std::endl;
                std::cout << "all chname : <" << it->second->getName() << ">" << std::endl;
                std::cout << "all cchname topic  : <" << it->second->getTopic() << ">" << std::endl;
                std::cout << "all ch operator  : <" << it->second->isOperator(_clientFd) << ">" << std::endl;
        }

    std::cout << "///registered channel///" << std::endl;
    std::map<std::string , IrcChannel*>::iterator it2;
    for (it2 = _channels.begin();
        it2 != _channels.end();
        it2++)
        {
            std::cout << "---------------------------" << std::endl;
            if (it2->second->isJoinedUser(_clientFd) == true)
            {
                std::cout << "clientFd : <" << _clientFd << ">" << std::endl;
                std::cout << "registerd chname : <" << it2->second->getName() << ">" << std::endl;
                std::cout << "registerd ch topic  : <" << it2->second->getTopic() << ">" << std::endl;
                std::cout << "registerd ch operator  : <" << it2->second->isOperator(_clientFd) << ">" << std::endl;
            }
        }
}
