#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcChannel.hpp"

void IrcCommand::MODE()
{

    std::cout << " ---------start MODE---------" << std::endl;
    
    
    
    //size check
    if (_args.size() < 2)
    {
        std::cout << "need more than 2 arguments" << std::endl;
        return;
    }
    //channel find check
    IrcChannel* channel = _db->findChannel(_args[0]);
    
    if (channel->isJoinedUser(_clientFd) == false)
    {
        throw ERR_NOTONCHANNEL();
    }
    //channel operator check
    if (channel->isOperator(_clientFd) == false)
    {
        std::cout << "<" << _db->findClientByFd(_clientFd)->getNickname() << "> is not a operator to <" << channel->getName() << ">" << std::endl;
        throw ERR_CHANOPRIVSNEEDED();
    }

    std::string option = _args[1];
    
    int mustOp = 0;///if mustOp == 0, next char have to be + or -
                    // if mustOp ==1, next char have to be option character
    int sign = 1;
    
    if (option[0] == '+')
    {
        sign = 1;
        mustOp = 1; //we have +, mustOP is setted to 1, then, next char have to be option char.
    }
    else if (option[0] == '-')
    {
        sign = 0;
        mustOp = 1;//we have -, mustOP is setted to 1, then, next char have to be option char.
    }
    else
    {
        std::cout << "options first letter have to be + or -"<<std::endl;
        throw ERR_UNKNOWNMODE();           
    }

    int argCount = 2;
    int i = 1;
    while(option[i])
    {
        int grant = channel->getGrant();

        if (option[i] == '+')
        {
            sign = 1;
            mustOp = 1; //we have +, mustOP is setted to 1, then, next char have to be option char.
        }
        else if (option[i] == '-')
        {
            sign = 0;
            mustOp = 1;//we have -, mustOP is setted to 1, then, next char have to be option char.
        }

        //in case, no + or -
        if (option[i] == 'i')
        {
            channel->setGrant(M_INVITE, sign);
        }
        else if (option[i] == 't')
        {
            channel->setGrant(M_TOPIC, sign);
        }
        else if (option[i] == 'k')
        {
            channel->setGrant(M_KEY, sign);
            if (sign)
                channel->setPassword(_args[argCount++]);
        }
        else if (option[i] == 'l')
        {
            channel->setGrant(M_LIMIT, sign);
            if (sign)
                std::cout << "setlimit fn need" << std::endl;      
        }
        else if (option[i] == 'o')
        {
            channel->setOperator(_clientFd, _db->findClientByName(_args[argCount++])->getFd());
        }
        else
        {
            std::cout << "there is no option : <" << option[i] << ">" << std::endl;
            throw ERR_UNKNOWNMODE();
        }               
        i++;
    }
    std::cout << "------MODE end------" << std::endl; 
}
