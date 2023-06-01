#include "../include/IRC.hpp"
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 1024 
char buf[BUF_SIZE];

IrcServ::IrcServ(int port, std::string passWord)
    : _port(port), _passWord(passWord)
{
    _servFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    _servAddr.sin_port=htons(port);
// setsockopt 추가
    if (_servFd == -1) 
        ErrorHandle::errorHandle("socket error", _servFd);
    if (bind(_servFd, (struct sockaddr*)&_servAddr, sizeof(_servAddr)))
        ErrorHandle::errorHandle("fail bind", -1);
    if (listen(_servFd, 5))
        ErrorHandle::errorHandle("fail listen", -1);

    FD_ZERO(&_activeReads);
    FD_ZERO(&_activeWrites);
}
#include <fcntl.h>

void IrcServ::run()
{

    int rcv_bytes;
    IrcClient ircClient;
    // IrcClient nextIrcClient;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while(true) {
        //sleep(3);
        FD_SET(_servFd, &_activeReads);
        FD_SET(_servFd, &_activeWrites);
        _fdMax = _servFd > GetMaxFd() ? _servFd : GetMaxFd() ;
        // _fdMax = _servFd;
        this->CopyClientFDtoFDSet(&this->_cpyReads);
        _activeReads = _cpyReads;

        std::cout << "fdMax\t:" << _fdMax << ", serverFd\t:" << _servFd << std::endl;
        select(_fdMax + 1, &this->_activeReads, NULL, NULL, NULL);
        std::cout << "hell" << std::endl;
        printf("after select\n");

        /* getNewConnection*/
         if (FD_ISSET(_servFd, &this->_activeReads)) 
         {
            printf("newconnection detected\n");
            int comm_socket_fd;
            comm_socket_fd =  accept (_servFd, (struct sockaddr *)&client_addr, &addr_len);
            //
            if (comm_socket_fd == -1)
                continue;
            _clients[comm_socket_fd] = IrcClient();
            //IrcClient *ircClient = new IrcClient();
            _clients[comm_socket_fd]._commFd = comm_socket_fd;
            _clients[comm_socket_fd]._ipAddr = htonl(client_addr.sin_addr.s_addr);
            _clients[comm_socket_fd]._portNo = htons(client_addr.sin_port);
            FD_SET(_clients[comm_socket_fd]._commFd, &_cpyReads);
            std::cout << _clients[comm_socket_fd]._commFd << ": this" << std::endl;
            // printf("ircClient_commfd in newconnect : %d  comm_socket_fd : %d\n", ircClient->_commFd, comm_socket_fd);
/*
            ircClient->_commFd = comm_socket_fd;
            printf("ircClient_commfd in newconnect : %d  comm_socket_fd : %d\n", ircClient->_commFd, comm_socket_fd);
            ircClient->_ipAddr = 
            ircClient->_portNo =
            FD_SET(ircClient->_commFd, &_cpyReads);
*/ 
         }        
        /* Iterate so that we can delete the current element while traversing */
        if (_clients.size() == 0)
            break;
        for (std::map<int, IrcClient>::iterator it = _clients.begin();
            it != _clients.end(); 
            it++)
        {
            printf("after for it\n");
            // printf("ircclient_fd is %d\n", ircClient->getCommFd());
            // if (FD_ISSET(ircClient->getCommFd(), &this->_activeReads)) 
            if (FD_ISSET(it->first, &_activeReads))
            {
                    printf("recv in commfd : %d\n", it->second.getCommFd());
                    
                    rcv_bytes = recvfrom(it->second.getCommFd(),
                            it->second.recv_buffer,
                            MAX_CLIENT_BUFFER_SIZE,
                            0, 
                            (struct sockaddr *)&client_addr, &addr_len);

                    if (rcv_bytes == 0 || rcv_bytes < 0) {
                        /* Remove FD from fd_set otherwise, select will go in infinite loop*/
                        FD_CLR(it->second.getCommFd(), &_cpyReads);
                        // FD_CLR(ircClient->getCommFd(), &this->_cpyReads);
                        // close(it->second._commFd);
                        close(it->first);
                        _clients.erase(it);
                        // this->RemoveClients(ircClient);
                        _fdMax = GetMaxFd();
                    }
                    else {

                        /*handle recv message*/
                    printf("MAX_CLIENT_BUFFER_SIZE : %d\n", MAX_CLIENT_BUFFER_SIZE);
                    printf ("%s() Bytes recvd : %d msg : %s\n",  __FUNCTION__, MAX_CLIENT_BUFFER_SIZE, it->second.recv_buffer);
                    //write(1, buf, rcv_bytes);
                    //write(1, "\n", 1);
                    }
            
            std::cout << "IrcServ::run() while loop" << std::endl;
            if (_clients.size() == 0)
                break;
            //break;
            }
            
        }
    }
}

IrcServ::~IrcServ() {};

int IrcServ::GetMaxFd() 
{
    int max_fd_lcl = 0;

    for (std::map<int, IrcClient>::iterator it = _clients.begin();
        it != _clients.end(); 
        it++)
    {
    // for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
        if (it->second.getCommFd() > max_fd_lcl ) {
            max_fd_lcl = it->second._commFd;
        }
    }
    return max_fd_lcl;
    // return (_clients.end()--)->first;
}


void IrcServ::CopyClientFDtoFDSet(fd_set *fdset) {

    for (std::map<int, IrcClient>::iterator it = _clients.begin();
        it != _clients.end(); 
        it++)
    {
        FD_SET(it->second._commFd, fdset);
    }
}

