#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcChannel.hpp"

void IrcCommand::MODE()
{

    std::cout << " start MODE" << std::endl;
    
    IrcChannel* channel = _db->findChannel(_args[0]);
    std::string option = _args[1];
    std::string arg = _args[2];
    std::cout << "arg : " << arg  << std::endl;

    if (option[1] == 'o')
    {
        int targetFd = _db->findClientByName(_args[2])->getFd();
        std::cout << "in MODE, targetFD : <" << targetFd << ">  clientFd : <" << _clientFd<< ">" << std::endl;
        

        channel->setOperator(_clientFd, _db->findClientByName(_args[2])->getFd());
    }
    else if (option[1] == 't')
    {

    }
    else if (option[1] == 'k')
    {
        
    }    
    else if (option[1] == 'l')
    {
        
    }
    else if (option[1] == 'i')
    {
        
    }
    else
    {
        std::cout << "wrong option in MODE" << std::endl;
    }
    std::cout << "------MODE end------" << std::endl; 
}