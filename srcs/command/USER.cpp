#include "../../include/IrcCommand.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcClient.hpp"
#include <string>
#include <vector>


//vector<std::string> jwee, hello, world

void IrcCommand::PONG(int clientFd){
    std::cout << "in <USER> command" << std::endl;
    //std::cout << "argus : <" << _args << ">" << std::endl;

    if (_args.size() != 4)
        return ;

    std::string newNickname;
    
    
    newNickname = *(_args.begin());
    
    
    
    IrcClient& client = find(clientFd);
    client.setNickname(newNickname);

    return ;
}