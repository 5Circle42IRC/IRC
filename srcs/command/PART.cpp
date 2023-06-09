#include "../../include/IrcCommand.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcClient.hpp"
#include <string>
#include <vector>


//vector<std::string> jwee, hello, world

void IrcCommand::PART(int clientFd){
    std::cout << "in <PART> command" << std::endl;
    //std::cout << "argus : <" << _args << ">" << std::endl;

    if (_args.size() != 1)
        return ;

    std::string target;
    
    
    target = *(_args.begin());
    const IrcChannel& channel = find(target);

     std::map<int, bool>::const_iterator it;

     for (it = channel.getUser().begin();
          it != channel.getUser().end();  
          it++)
        {
            if (it->first == clientFd )
            {
                deleteUser(clientFd);
                break;
            }
        }
    
    }
