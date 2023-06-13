#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::USER(int clientfd){
    if (getArgs().size() != 4)
        std::cout << "in USER command, args size is not 4. " << std::endl;
    else    
    {
        _db->findClientByFd(clientfd)->setUsername(getArgs()[0]);
        _db->findClientByFd(clientfd)->setHostname(getArgs()[1]);
        _db->findClientByFd(clientfd)->setServername(getArgs()[2]);
        _db->findClientByFd(clientfd)->setRealname(getArgs()[3]);
    }
}