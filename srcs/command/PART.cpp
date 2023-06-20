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
            ERR_NEED_MORE_PARAMS (461) 
            "<client> <command> :Not enough parameters"
        */
        throw ERR_NEED_MORE_PARAMS();
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
        chname = _args[i]; 
        channel =_db->findChannel(*it);
        /*
            ERR_NOSUCHCHANNEL (403) 
            "<client> <channel> :No such channel"        
        */



        int size = chname.size();
        std::cout << "chname : <" << chname << ">" << std::endl;
        std::cout << "chname size : <" << size << ">" << std::endl;
        /*
            ERR_NOT_ON_CHANNEL (442) 
            "<client> <channel> :You're not on that channel"
        
        */
        if (channel->isJoinedUser(_clientFd) == false)
        {
            std::cout << "clientfd : <" << _clientFd << "> is not joinned to <" << channel->getName() << ">" << std::endl;   
            throw ERR_NOT_ON_CHANNEL();
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
        i++;
        client->addBackBuffer(client->getNickname() + ": PART " + chnameSum + "\r\n");
    }
    return ;
}