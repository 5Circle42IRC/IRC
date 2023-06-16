#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
<<<<<<< HEAD
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
    
    //channel operator check
    if (channel->isOperator(_clientFd) == false)
    {
        std::cout << "<" << _db->findClientByFd(_clientFd)->getNickname() << "> is not a operator to <" << channel->getName() << ">" << std::endl;
        return;      
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
        return;              
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
            return ;
        }               
        i++;
    }

    /*
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
    */
    std::cout << "------MODE end------" << std::endl; 
}
=======

static int checkValidMODE(std::deque<std::string>		args, IrcDB *_db)
{
    std::string nick = args[0]; 
    if (nick.size() > 9)
    {
        std::cout << "NICK size have to be lower than 9" << std::endl;
        throw std::exception();
    }
    if (_db->findClientByName(nick)->getNickname() == nick)
    {
        std::cout << "Already same nick exists." << std::endl;
        std::cout << "nick <" << nick << ">  " << "ClientFd : <" << _db->findClientByName(nick)->getFd() << ">" << std::endl;
        throw std::exception();        
    } 
    return 1;
}

void IrcCommand::MODE(int clientfd){
    
        if (getArgs().size() != 1)
            std::cout << "NICK args size is not 1" << std::endl;
        else
        {
            if (checkValidNICK(getArgs(), _db) == 1 )
                _db->findClientByFd(clientfd)->setNickname(getArgs()[0]);
        }    
}
>>>>>>> 4148e28f0618c9aecb9214cb85b37f79067cc4a5
