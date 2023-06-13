#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcDB.hpp"

void IrcCommand::PRIVMSG(int clientfd){
    std::string firstArg = _args[0];
    std::string msg = _args[1];

    std::cout << "firstArg : <" << firstArg << ">  msg : <" << msg << "> _args.front() : <" << firstArg.front() << ">" << std::endl;
    IrcClient *client = _db->findClientByName(firstArg);
    std::cout << "client fd : <" << client->getFd() << ">  nickname : <" << client->getNickname() << ">" << std::endl;
    client->addBackBuffer(":" + client->getNickname() + " PRIVMSG " + client->getNickname() + " :" + msg);
}