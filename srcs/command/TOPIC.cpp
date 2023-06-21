#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcChannel.hpp"

void IrcCommand::TOPIC(){
     
    if (_args.size() > 2)
    {
        std::cout << "TOPIC args size is not 2" << std::endl;   
        client->addBackBuffer("461 <" + _command + "> ");
        throw ERR_NEEDMOREPARAMS();
    }
    std::string chname = _args[0];
    std::string topic = _args[1];
    IrcChannel* channel = _db->findChannel(chname);
    IrcClient* client = _db->findClientByFd(_clientFd);

    if (channel->isJoinedUser(_clientFd) == false)
    {
        std::cout << "clientfd : <" << _clientFd << "> is not joinned to <" << channel->getName() << ">" << std::endl;   
        client->addBackBuffer(":localhost 442 " + client->getNickname() + " " + channel->getName());
        throw ERR_NOTONCHANNEL();
        /*
            ERR_NOTONCHANNEL (442) 
            "<client> <channel> :You're not on that channel"
        
        */
    }

    if (_args.size() == 1)
    {
        std::cout << "topic test1" << std::endl;
        client->addBackBuffer(":"+client->getNickname() +" 332 " + client->getNickname() + " " + channel->getName()+ " " + channel->getTopic() +"\r\n");
    }
    else if (_args.size() == 2)
    {
        if (channel->isOperator(_clientFd) == false)
        {
            std::cout << "Don't have privillege to change topic" << std::endl;
            client->addBackBuffer(":localhost 482 " + client->getNickname() + " " + channel->getName());
            throw ERR_CHANOPRIVSNEEDED();
            /*
                ERR_CHANOPRIVSNEEDED (482) 
                "<client> <channel> :You're not channel operator"
            */
        }
        else
        {

            std::cout << "Before change TOPIC : <" << channel->getTopic() << ">" << std::endl;
            channel->setTopic(topic);
            std::cout << "After change TOPIC : <" << channel->getTopic() << ">" << std::endl; 
            std::cout << "after topic change; hannel->getTopic().size() : <" << channel->getTopic().size() << ">" << std::endl;
            if (channel->getTopic().size() == 0)//채널 생성시 토픽의 사이즈는???
                {
                    std::cout << "topic test2" << std::endl;    
                    client->addBackBuffer(client->getNickname() + " " + channel->getName() + " :No topic is set" + "\r\n");
                }
            else
                {
                    std::cout << "topic test3" << std::endl;
                    client->addBackBuffer(":" + client->getNickname() + " TOPIC " + client->getNickname() + " :" + channel->getName()+ " " + topic +"\r\n");
                
                }
        }
    }
}