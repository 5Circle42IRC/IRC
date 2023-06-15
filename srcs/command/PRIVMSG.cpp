#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcDB.hpp"

void IrcCommand::PRIVMSG(){
    std::string firstArg = _args[0];
    std::string msg = _args[1];

    
    std::cout << "firstArg : <" << firstArg << ">  msg : <" << msg << "> _args.front() : <" << firstArg.front() << ">" << std::endl;
    IrcClient *client = _db->findClientByFd(_clientFd);
    IrcClient *target = _db->findClientByName(firstArg);
    std::cout << "client fd : <" << client->getFd() << ">  nickname : <" << client->getNickname() << ">" << std::endl;
    std::cout << "target fd : <" << target->getFd() << ">  nickname : <" << target->getNickname() << ">" << std::endl;


    //:Angel PRIVMSG Wiz :Hello are you receiving this message ?
    std::cout << "bf client buffer : <" << client->getBuffer() << ">" << std::endl;
    std::cout << "bf target buffer : <" << target->getBuffer() << ">" << std::endl;
    target->addBackBuffer(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg);

    //PRIVMSG Angel :
    client->addBackBuffer("PRIVMSG " + target->getNickname() + " :" + msg);
    std::cout << "af client buffer : <" << client->getBuffer() << ">" << std::endl;
    std::cout << "af target buffer : <" << target->getBuffer() << ">" << std::endl;
}