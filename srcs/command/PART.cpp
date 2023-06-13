#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"

static int checkValidPART(std::deque<std::string>		args)
{
    std::string chname = args[0]; 
    int size = chname.size();

    if (args.size() < 1)
    {
        std::cout << "PART need more than 1 args.  args size : <" << args[0].size() << ">" << std::endl;
        throw std::exception();
    }
    if (size > 200 || size < 2)
    {   
        std::cout << "chname size is wrong, chname : <" << chname << ">" << std::endl;
        throw std::exception();
    }
    if (chname.front() != '#')
    {
        std::cout << "chname not start with #,   chname : <" << chname << ">" << std::endl;
        throw std::exception();
    }

    return 1;
}

void IrcCommand::PART(int clientFd){
    try
    {
        IrcChannel *channel;
        IrcClient *client;
        std::string chname = _args[0]; 

        client = _db->findClientByFd(clientFd);
        channel =_db->findChannel(chname);
        if (checkValidPART(_args))//check chname valid
        {
            channel = _db->findChannel(chname);
            if (channel->deleteUser(clientFd) == false)
                throw std::exception();
        }
        client->addBackBuffer(client->getNickname() + ": PART " + channel->getName());
        return ;
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception in PART: < " << e.what() << ">" << std::endl;
        return ;
    }   
}