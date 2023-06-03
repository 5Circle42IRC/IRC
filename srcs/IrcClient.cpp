#include <arpa/inet.h>
#include "../include/IRC.hpp"

IrcClient::IrcClient(int commFd, uint32_t ipAddr, uint16_t portNo)
    : _commFd(commFd), _ipAddr(ipAddr), _portNo(portNo)
{ }

IrcClient::~IrcClient() { }

IrcClient::IrcClient(const IrcClient& copy)
{
    *this = copy;
}

const IrcClient& IrcClient::operator=(const IrcClient& copy)
{
    if (this == &copy)
    {
        this->_commFd = copy._commFd;
        this->_ipAddr= copy._ipAddr;
        this->_portNo= copy._portNo;
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

