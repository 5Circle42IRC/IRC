#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::NICK(int clientfd){

    if (getArgs().size() != 1)
        std::cout << "NICK args size is not 1" << std::endl;
    else
    {
        //if (getArgs()[0] != NULL)//nick valid check
        _db->findClientByFd(clientfd)->setNickname(getArgs()[0]);
    }
}
