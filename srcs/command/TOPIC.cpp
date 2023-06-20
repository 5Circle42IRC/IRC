#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcDB.hpp"
#include "../../include/IrcChannel.hpp"

void IrcCommand::TOPIC(){
     
    if (_args.size() != 2)
    {
        std::cout << "TOPIC args size is not 2" << std::endl;   
        throw ERR_NEEDMOREPARAMS();
    }
    std::string chname = _args[0];
    std::string topic = _args[1];
    IrcChannel* channel = _db->findChannel(chname);/*
                                                        ERR_NOSUCHCHANNEL (403) 
                                                        "<client> <channel> :No such channel"
                                                    */

    IrcClient* client = _db->findClientByFd(_clientFd);

    std::cout << "TOPIC SIZE befe : <" << channel->getTopic().size() << ">" << std::endl;
    std::cout << "TOPIC : <" << channel->getTopic() << ">" << std::endl;
    if (channel->isJoinedUser(_clientFd) == false)
    {
        std::cout << "clientfd : <" << _clientFd << "> is not joinned to <" << channel->getName() << ">" << std::endl;   
        throw ERR_NOTONCHANNEL();
        /*
            ERR_NOTONCHANNEL (442) 
            "<client> <channel> :You're not on that channel"
        
        */
    }
    if (channel->isOperator(_clientFd) == false)
    {
        std::cout << "Don't have privillege to change topic" << std::endl;
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

        if (channel->getTopic().size() == 0)//채널 생성시 토픽의 사이즈는???
            //"<client> <channel> :No topic is set"
            
            client->addBackBuffer(client->getNickname() + " " + channel->getName() + " :No topic is set" + "\r\n");
        else
            //"<client> <channel> :<topic>"
            client->addBackBuffer(":" + client->getNickname() + " TOPIC :" + channel->getName()+ " :" + topic +"\r\n");
            //client->addBackBuffer(client->getNickname() + " " + channel->getName() + " :" + channel->getTopic() + "\r\n");
    }
}