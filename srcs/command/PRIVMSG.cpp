#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"
#include "../../include/IrcDB.hpp"

void IrcCommand::PRIVMSG(){

    if (_args.size() < 2)
    {
        throw ERR_NEEDMOREPARAMS();
    }
    std::string firstArg = _args[0];
    std::string msg = _args[1];

    if (firstArg.front() == '#')    
    {
        std::cout << "In PRIVMSG # start, so channel" << std::endl;
        IrcChannel* channel = _db->findChannel(firstArg);
        IrcClient* client = _db->findClientByFd(_clientFd);
        
        std::map<int, bool>	users = channel->getUser();
        std::map<int, bool>::iterator it;

        if (!channel->isJoinedUser(_clientFd))
            throw ERR_NOTONCHANNEL();
        for (it = users.begin();
                it != users.end();
                it++)
                {
                    IrcClient *client = _db->findClientByFd(_clientFd);
                    IrcClient *target = _db->findClientByFd(it->first);                    
                    if (_clientFd != target->getFd())
                    {
                         
                        target->addBackCarriageBuffer(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg);
                        client->addBackCarriageBuffer(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg);
                    }
                }      
    }
    else
    {
            IrcClient *client = _db->findClientByFd(_clientFd);
            IrcClient *target = _db->findClientByName(firstArg);        
        if (_clientFd != target->getFd())
        {
            target->addBackCarriageBuffer(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg);
            client->addBackCarriageBuffer(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg);
        }
    }
}