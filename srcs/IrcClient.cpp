#include <arpa/inet.h>
#include "../include/IRC.hpp"

IrcClient::IrcClient(int commFd, sockaddr_in addr, socklen_t addrLen)
    : _commFd(commFd), _addr(addr), _addrLen(addrLen)
{ }

IrcClient::IrcClient()
    : _commFd(0)
{ }

IrcClient::~IrcClient() { }

IrcClient::IrcClient(const IrcClient& copy)
    : _commFd(copy._commFd), _addr(copy._addr), _addrLen(copy._addrLen)
{ }

const IrcClient& IrcClient::operator=(const IrcClient& copy)
{
    if (this == &copy)
    {
        this->_commFd = copy._commFd;
        this->_addr = copy._addr;
        this->_addrLen = copy._addrLen;
    }
    return (*this);
}

void IrcClient::Display() {

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

