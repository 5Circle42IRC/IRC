#include "../../include/IrcCommand.hpp"
#include <string>
#include <vector>


//vector<std::string> jwee, hello, world

void execute(std::vector<std::string> args){
    std::cout << "in <PRIVMSG> command" << std::endl;
    //std::cout << "argus : <" << args << ">" << std::endl;

    if (args.size() < 2)
        return ;

    std::string target;
    std::vector<std::string>::iterator it;


    it = args.begin();
    target = *it;

    std::string msg;
    for (it = std::next(args.begin());
        it != args.end();
        it++)
        {
            msg += *it;
            msg += " ";
        }

    std::cout << "msg : " << msg << std::endl;
    if (target.at(0) == '#')
    {
        std::cout << "target is # start, so CHANNEL" << std::endl;
        std::map<int, bool>::iterator it2;
        for(it2 = users.begin();
            it2 != users.end();
            it2++)      
            {
                //send((*it2)->getCommFd(), msg.c_str(), msg.size()+1, 0);
                std::cout << "send msg to channel : " << it2->first << " send msg : " << msg << std::endl;
            }
    }
    else
    {
        std::cout << "target is client" << std::endl;
        
        std::map<int, IrcClient &>::iterator it3;
        
        for(it3 = _clients.begin();
            it3 != _clients.end();
            it3++)
            {
                if (it3->second.getNickname()== target)
                    //send((*it3)->getCommFd(), &msg, msg.size()+1, 0);
                    std::cout << "send msg to nick : " << target << std::endl;
            }
        }
        
    }
