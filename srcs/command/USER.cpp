#include "../../include/IrcCommand.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcClient.hpp"
#include <string>
#include <vector>
#include <map>

//vector<std::string> jwee, hello, world

void IrcCommand::USER(int clientFd){
    std::cout << "in <USER> command" << std::endl;
    //std::cout << "argus : <" << _args << ">" << std::endl;

    if (_args.size() != 4)
        return ;
    std::string username = _args[0];
    std::string hostname = _args[1];
    std::string servername = _args[2];
    std::string realname = _args[3];
    
    
    IrcClient& client = find(clientFd);
    
    client.setUsername(username);
    client.setHostname(hostname);
    client.setServername(servername);
    client.setRealname(realname);
    return ;
}