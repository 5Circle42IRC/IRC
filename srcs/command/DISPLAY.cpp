#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcDB.hpp"
void IrcCommand::DISPLAY(){
    
    IrcClient* client = _db->findClientByFd(_clientFd);
    std::cout << "------------Display -------" << std::endl;
    std::cout << "Nick : " << client->getNickname() << std::endl;
    std::cout << "UserName : " << client->getUsername() << std::endl;
    std::cout << "HostName : " << client->getHostname() << std::endl;
    std::cout << "ServerName : " << client->getServername() << std::endl;
    std::cout << "RealName : " << client->getRealname() << std::endl;
}
