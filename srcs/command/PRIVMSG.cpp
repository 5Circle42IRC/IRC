#include "../../include/IrcCommand.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcClient.hpp"
#include <string>
#include <vector>


//vector<std::string> jwee, hello, world

void IrcCommand::PRIVMSG(){
    std::cout << "in <PRIVMSG> command" << std::endl;
    //std::cout << "argus : <" << _args << ">" << std::endl;

    if (_args.size() != 2)
        return ;

    std::string target;
    std::vector<std::string>::iterator it;


    it = _args.begin();
    target = *it;

    std::string msg = *(_args.end());
	/*
    for (it = std::next(_args.begin());
        it != _args.end();
        it++)
        {
            msg += *it;
            msg += " ";
        }
	*/
    std::cout << "msg : " << msg << std::endl;
    if (target.at(0) == '#')
    {
        std::cout << "target is # start, so CHANNEL" << std::endl;
        std::map<int, bool>::const_iterator it2;
		const IrcChannel& channel = find(target);
        for(it2 = channel.getUser().begin();
            it2 != channel.getUser().end();
            it2++)      
            {
                //send((*it2)->getCommFd(), msg.c_str(), msg.size()+1, 0);
				IrcClient& client = find(it2->first);
				client.addBackBuffer(msg);
                //std::cout << "send msg to channel : " << it2->first << " send msg : " << msg << std::endl;
            }
    }
    else
    {
        std::cout << "target is client" << std::endl;
        
        std::map<int, IrcClient &>::iterator it3;
        
        for(it3 = _client.begin();
            it3 != _client.end();
            it3++)
            {
                if (it3->second.getNickname()== target)
                    //send((*it3)->getCommFd(), &msg, msg.size()+1, 0);
					it3->second.addBackBuffer(msg);
                    //std::cout << "send msg to nick : " << target << std::endl;
            }
        }
        
    }
