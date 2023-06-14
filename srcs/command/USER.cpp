#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::USER(){
    if (getArgs().size() != 4)
        std::cout << "in USER command, args size is not 4. " << std::endl;
    else    
    {
        _db->findClientByFd(_clientFd)->setUsername(getArgs()[0]);
        _db->findClientByFd(_clientFd)->setHostname(getArgs()[1]);
        _db->findClientByFd(_clientFd)->setServername(getArgs()[2]);
        _db->findClientByFd(_clientFd)->setRealname(getArgs()[3]);
    }
}