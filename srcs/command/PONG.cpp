#include "../../include/IrcCommand.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcClient.hpp"
#include <string>
#include <vector>


//vector<std::string> jwee, hello, world

void IrcCommand::PONG(int clientFd){
    std::cout << "in <PONG> command" << std::endl;
    //std::cout << "argus : <" << _args << ">" << std::endl;

    if (_args.size() != 1)
        return ;

    std::string pongMsg("Pong MSG");
    
    
    
    
    
    
    IrcClient& client = find(clientFd);
    client.addBackBuffer(pongMsg);

    return ;
}