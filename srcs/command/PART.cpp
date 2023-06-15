#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"



void IrcCommand::PART(){
    IrcChannel *channel;
    IrcClient *client;

    if (_args.size() < 1)
    {
        std::cout << "PART need more than 1 args.  args size : <" << _args[0].size() << ">" << std::endl;
        /*
            ERR_NEEDMOREPARAMS (461) 
            "<client> <command> :Not enough parameters"
        */
        throw std::exception();
    }


    

    client = _db->findClientByFd(_clientFd);
    int i = 0;


    std::string chname;
    std::string chnameSum;

    while (i < _args.size())
    {

        chname = _args[i]; 
        channel =_db->findChannel(chname);


        int size = chname.size();
        std::cout << "chname : <" << chname << ">" << std::endl;
        std::cout << "chname size : <" << size << ">" << std::endl;
        /*
            ERR_NOSUCHCHANNEL (403) 
            "<client> <channel> :No such channel"        
        */
        channel = _db->findChannel(chname);
        
        
        ///my own error
        if (size > 200 || size < 2)
        {   
            std::cout << "chname size is wrong, chname : <" << chname << ">" << std::endl;
            throw std::exception();
        }  


        /*
            ERR_NOTONCHANNEL (442) 
            "<client> <channel> :You're not on that channel"
        
        */
        if (channel->isJoinedUser(_clientFd) == false)
        {
            std::cout << "clientfd : <" << _clientFd << "> is not joinned to <" << channel->getName() << ">" << std::endl;   
            throw std::exception();
        }              

        ///my own error
        if (channel->deleteUser(_clientFd) == true)
        {
            std::cout << "PART success" << std::endl;
        }
        else
        {
            std::cout << "PART fail???" << std::endl;
            throw std::exception();
        }
        chnameSum += (chname + " ");
        i++;
    }
    client->addBackBuffer(client->getNickname() + ": PART " + chnameSum);
    return ;
}