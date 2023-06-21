#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"



void IrcCommand::PART(){
    IrcChannel *channel;
    IrcClient *client;

    std::vector<std::string> channelList;

    if (_args.size() != 1)
    {
        std::cout << "PART need more than 1 args.  args size : <" << _args[0].size() << ">" << std::endl;
        /*
            ERR_NEEDMOREPARAMS (461) 
            "<client> <command> :Not enough parameters"
        */
        client->addBackBuffer("461 <" + _command + "> ");
        throw ERR_NEEDMOREPARAMS();
    }

    for (int end = _args[0].find(","); end != -1; end = _args[0].find(",")){
		channelList.push_back(_args[0].substr(0, end));
		_args[0].erase(0, end + 1);
	}
	channelList.push_back(_args[0]);

    

    client = _db->findClientByFd(_clientFd);

    int i = 0;


    std::string chname;
    std::string chnameSum;

    for (std::vector<std::string>::iterator it = channelList.begin(); it != channelList.end(); it++){
       
        channel =_db->findChannel(*it);
        chname = channel->getName(); 
        /*
            ERR_NOSUCHCHANNEL (403) 
            "<client> <channel> :No such channel"        
        */



        int size = chname.size();
        std::cout << "chname : <" << chname << ">" << std::endl;
        std::cout << "chname size : <" << size << ">" << std::endl;
        /*
            ERR_NOTONCHANNEL (442) 
            "<client> <channel> :You're not on that channel"
        
        */
        if (channel->isJoinedUser(_clientFd) == false)
        {
            std::cout << "clientfd : <" << _clientFd << "> is not joinned to <" << channel->getName() << ">" << std::endl;   
            client->addBackBuffer(":localhost 442 " + client->getNickname() + " " + channel->getName());
            throw ERR_NOTONCHANNEL();
        }              

        ///my own error
        if (channel->deleteUser(_clientFd) == true)
        {
            std::cout << "PART success <" << _clientFd << "> from <" << channel->getName() << ">" << std::endl;
            if (channel->getUser().size() == 0)
                _db->deleteChannel(channel->getName());
        }
        else
        {
            std::cout << "PART Fail <" << _clientFd << "> from <" << channel->getName() << ">" << std::endl;
            throw std::exception();
        }
        chnameSum += (chname + " ");
        client->addBackBuffer(":" + client->getNickname() + " PART :" + chname + "\r\n");
        i++;
        
    }
    //client->addBackBuffer(":" + client->getNickname() + " PART :" + chnameSum + "\r\n");
    return ;
}