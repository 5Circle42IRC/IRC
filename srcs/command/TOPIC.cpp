#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcChannel.hpp"

static int checkValidTOPIC(std::deque<std::string>		args, IrcDB *_db)
{
    if (args.size() != 2)
    {
        std::cout << "TOPIC args size is not 2" << std::endl;   
        throw std::exception();
    }



  
    return 0;
}
void IrcCommand::TOPIC(int clientFd){
     
    if (checkValidTOPIC(getArgs(), _db) == 0)
        throw std::exception();
    
    std::string chname = _args[0];
    std::string topic = _args[1];
    IrcChannel* channel = _db->findChannel(chname);
    IrcClient* client = _db->findClientByFd(clientFd);      

    if (channel->isOperator(clientFd))
    {
        std::cout << "Before change TOPIC : <" << channel->getTopic() << ">" << std::endl;
        channel->setTopic(topic);
        std::cout << "After change TOPIC : <" << channel->getTopic() << ">" << std::endl;
    }
    else
        std::cout << "Don't have privillege to change topic" << std::endl;

}