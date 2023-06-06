#include <arpa/inet.h>
#include "../include/IRC.hpp"

IrcClient::IrcClient(uint32_t ipAddr, uint16_t portNo) {

    this->_ipAddr = ipAddr;
    this->_portNo = portNo;
    this->nickFlag = false;
    this->userFlag = false;
    this->passFlag = false;
    this->registFlag = false;
    
    bzero(this->recv_buffer, MAX_CLIENT_BUFFER_SIZE);
}


IrcClient::IrcClient() {

    this->_ipAddr = 0;
    this->_portNo = 0;
    this->nickFlag = false;
    this->userFlag = false;
    this->passFlag = false;
    this->registFlag = false;
    bzero(this->recv_buffer, MAX_CLIENT_BUFFER_SIZE);
}
int IrcClient::getCommFd()
{
    return this->_commFd;
}
void IrcClient::Display() {
    std::cout << "IN Display" << std::endl;
    std::cout << "object address : " << this << std::endl;
    std::cout << "NICK : " << getNick() << std::endl;
    std::cout << "USER username  : " << getUsername() << std::endl;
    std::cout << "HOST username  : " << getHostname() << std::endl;
    std::cout << "SERVER username  : " << getServername() << std::endl;
    std::cout << "REAL username  : " << getRealname() << std::endl;
    std::cout << "USER flag : " << getUserFlag() << std::endl;    
    std::cout << "PASS flag : " << getPassFlag() << std::endl;
    std::cout << "NICK flag : " << getNickFlag() << std::endl;
    std::cout << "Regs flag : " << getRegistFlag() << std::endl;
    std::cout << " --------Channel-------" << std::endl;
    std::cout << "channel size : " << this->_registredChannels.size() << std::endl;
    std::vector<IrcChannel>::iterator it;
    int i = 0;
    for (it = this->_registredChannels.begin();
        it != this->_registredChannels.end();
        it++)
        {
            i++;
            std::cout << "_registredChannels[" << i << "].name : " << it->getChannelName() << std::endl;
        }
    /*
    char ipAddrStr1[16];
    char ipAddrStr2[16];
    
    printf ("Irc Client : [%s , %d] connected to [%s, %d]\n", 
        network_convert_ip_n_to_p(this->ip_addr, ip_addr_str1),
        (this->port_no),
        network_convert_ip_n_to_p(this->server_ip_addr, ip_addr_str2),
        (this->server_port_no)
        );
    */
}

void IrcClient::setNick(std::string nick)
{
    
    this->nick = nick;
    this->nickFlag = true;
}

std::string IrcClient::getNick()
{
    return this->nick;
}

bool IrcClient::getNickFlag()
{
    return this->nickFlag;
}


void IrcClient::setUsername(std::string username)
{
    this->username = username;
}

std::string IrcClient::getUsername()
{
    return this->username;
}

void IrcClient::setHostname(std::string hostname)
{
    this->hostname = hostname;
}

std::string IrcClient::getHostname()
{
    return this->hostname;
}

void IrcClient::setServername(std::string servername)
{
    this->servername = servername;
}

std::string IrcClient::getServername()
{
    return this->servername;
}

void IrcClient::setRealname(std::string realname)
{
    this->realname = realname;
}

std::string IrcClient::getRealname()
{
    return this->realname;
}

void IrcClient::setUserFlag()
{
    this->userFlag = true;

}

bool IrcClient::getUserFlag()
{
    return this->userFlag;
}

bool IrcClient::checkPASS(std::string pass, std::string serverpass)
{
    std::cout << "      pass : <" << pass       << ">" << std::endl;
    std::cout << "serverpass : <" << serverpass << ">" << std::endl;
    if (pass == serverpass)
        return true;
    return false;
}

void IrcClient::setPassFlag()
{
    this->passFlag = true;
}

bool IrcClient::getPassFlag()
{
    return this->passFlag;
}

void IrcClient::setRegistFlag()
{
    this->registFlag = true;
}

bool IrcClient::getRegistFlag()
{
    return this->registFlag;
}


bool IrcClient::checkPart(std::string channelname)
{
    std::vector<IrcChannel>::iterator it;
    
    for (it = this->_registredChannels.begin();
            it != this->_registredChannels.end();
            it++)
        if (it->getChannelName() == channelname)
            return true;
            
    return false;
}

void IrcClient::doPart(std::string channelname)
{
    std::vector<IrcChannel>::iterator it;
    

    for (it = this->_registredChannels.begin();
        it != this->_registredChannels.end(); it++)
    {
        // remove odd numbers
        if (it->getChannelName() == channelname)
        {
            // Notice that the iterator is decremented after it is passed
            // to `erase()` but before `erase()` is executed
            std::cout << "here i delete : " << it->getChannelName() << std::endl;
            this->_registredChannels.erase(it--);
        }
    }
}