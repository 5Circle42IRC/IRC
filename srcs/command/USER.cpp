#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::USER(){

    IrcClient* client = _db->findClientByFd(_clientFd);
    std::string oldNick = client->getNickname();
    if (getArgs().size() != 4)
    {
        std::cout << "in USER command, args size is not 4. " << std::endl;
        client->addBackBuffer("461 <" + _command + "> ");
        throw ERR_NEEDMOREPARAMS();
    }
    else    
    {
        _db->findClientByFd(_clientFd)->setUsername(getArgs()[0]);
        _db->findClientByFd(_clientFd)->setHostname(getArgs()[1]);
        _db->findClientByFd(_clientFd)->setServername(getArgs()[2]);
        _db->findClientByFd(_clientFd)->setRealname(getArgs()[3]);

        /*
            :testnick USER guest tolmoon tolsun :Ronnie Reagan
        */
        client->addBackBuffer(":" + oldNick + " USER " + getArgs()[0] + " " + getArgs()[1] + " " + getArgs()[2] + " " + getArgs()[3] +  "\n");
    }
}