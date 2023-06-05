#include "../include/IRC.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
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

    int opt = 1;

// setsockopt 추가
    if (_servFd == -1) 
        ErrorHandle::errorHandle("socket error", _servFd);
    if (bind(_servFd, (struct sockaddr*)&_servAddr, sizeof(_servAddr)))
        ErrorHandle::errorHandle("fail bind", -1);
    if (listen(_servFd, 15))
        ErrorHandle::errorHandle("fail listen", -1);

    FD_ZERO(&_activeReads);
    FD_ZERO(&_activeWrites);
}


void IrcServ::run()
{

    int rcv_bytes;
    IrcClient *ircClient, *nextIrcClient;
    struct sockaddr_in client_addr;
    std::list<IrcClient *>::iterator it;
    socklen_t addr_len = sizeof(client_addr);


    while(true) {
        //sleep(3);
        struct timeval nulltime;
        nulltime.tv_sec = 2;
        nulltime.tv_usec = 0;  
              
        FD_SET(_servFd, &_cpyReads);
        FD_SET(_servFd, &_cpyWrites);
        _fdMax = _servFd > GetMaxFd() ? _servFd : GetMaxFd() ;
        this->CopyClientFDtoFDSet(&this->_cpyReads);
        _activeReads = _cpyReads;
        printf("_servFd is %d port is %d  fdMax is %d \n", _servFd, _port, _fdMax);
        select(_fdMax+1 , &this->_activeReads, NULL, NULL, NULL);
        printf("after select\n");

        /* getNewConnection*/
         if (FD_ISSET(_servFd, &this->_activeReads)) 
         {
            printf("newconnection detected\n");
            int comm_socket_fd;
            comm_socket_fd =  accept (_servFd,
                                                     (struct sockaddr *)&client_addr, &addr_len);
            IrcClient *ircClient = new IrcClient();
            ircClient->_commFd = comm_socket_fd;
            printf("ircClient_commfd in newconnect : %d  comm_socket_fd : %d\n", ircClient->_commFd, comm_socket_fd);
            ircClient->_ipAddr = htonl(client_addr.sin_addr.s_addr);
            ircClient->_portNo = htons(client_addr.sin_port);
            FD_SET(ircClient->_commFd, &_cpyReads);
            AddClients(ircClient);
         }        
        /* Iterate so that we can delete the current element while traversing */

        for (it = this->_clients.begin(), ircClient = *it;
             it != this->_clients.end();
             ircClient = nextIrcClient)
             {

                nextIrcClient = *(++it);



  
            printf("after for it\n");


            printf("ircclient_fd is %d\n", ircClient->getCommFd());
            if (FD_ISSET(ircClient->getCommFd(), &this->_activeReads)) 
            {
                    printf("recv in commfd : %d\n", ircClient->getCommFd());
                    
                    rcv_bytes = recvfrom(ircClient->getCommFd(),
                            ircClient->recv_buffer,
                            MAX_CLIENT_BUFFER_SIZE,
                            0, 
                            (struct sockaddr *)&client_addr, &addr_len);

                    if (rcv_bytes == 0 || rcv_bytes < 0) {
                        /* Remove FD from fd_set otherwise, select will go in infinite loop*/

                        printf("_____________________client remove   fd: %d\n", ircClient->getCommFd());
                        FD_CLR(ircClient->getCommFd(), &this->_cpyReads);
                        this->RemoveClients(ircClient);
                        printf("_____________________client remove2   fd: %d\n", ircClient->getCommFd());
                        _fdMax = GetMaxFd();
                        printf("______________________fdMax   fd: %d\n", GetMaxFd());
                    }
                    else {

                        /*handle recv message*/
                    printf("MAX_CLIENT_BUFFER_SIZE : %d\n", MAX_CLIENT_BUFFER_SIZE);
                    printf ("%s() Bytes recvd : %d msg : %s\n",  __FUNCTION__, MAX_CLIENT_BUFFER_SIZE, ircClient->recv_buffer);
                    CommandHandler(ircClient);
                    ircClient->Display();
                    //write(1, buf, rcv_bytes);
                    //write(1, "\n", 1);
                    }
            
            std::cout << "IrcServ::run() while loop" << std::endl;
            //break;
            }
            
        }
    }
}

void IrcServ::CommandHandler(IrcClient *ircClient)
{
    std::cout << "-----------------In Handler-------------" << std::endl;
    std::string message;
    
	message.append(ircClient->recv_buffer);
    message.erase(message.size() - 1);
    std::cout << "message : " << message << std::endl;
    std::cout << "message size : " << message.size() << std::endl;

    if (ircClient->getUserFlag() && ircClient->getPassFlag() && ircClient->getNickFlag())
        ircClient->setRegistFlag();
    
    if (message.find(" ") != std::string::npos) 
    {
        std::string command_name = message.substr(0, message.find(' '));
        std::string argus = message.substr(message.find(' ')+1, message.size());
        std::cout << "message size : " << message.size() << " find : " << message.find(' ') << std::endl;
        std::cout << "command_name : " << command_name << std::endl;
        if (command_name == "NICK")
            nick_command(ircClient, argus);
        else if (command_name == "USER")
            user_command(ircClient, argus);
        else if (command_name == "PASS")
            pass_command(ircClient, argus);
        else
            std::cout << "not invalid command" << std::endl;
    }
    else
    {
        std::cout << " no space in message " << std::endl;
    }
    bzero(ircClient->recv_buffer, MAX_CLIENT_BUFFER_SIZE);
};

void IrcServ::nick_command(IrcClient *ircClient, std::string argus) 
{
    std::cout << "in <NICK> command" << std::endl;
    std::cout << "argus : <" << argus << ">" << std::endl;
    if (argus.find(" ") != std::string::npos) 
        std::cout << "No need SPACE in NICK, find : " << argus.find(" ") << std::endl;
    else
        ircClient->setNick(argus);
    if (ircClient->getUserFlag() && ircClient->getPassFlag() && ircClient->getNickFlag())
        ircClient->setRegistFlag();
           
};

void IrcServ::user_command(IrcClient *ircClient, std::string argus) 
{
    std::cout << "in <USER> command" << std::endl;
    std::cout << "argus : <" << argus << ">" << std::endl;

    std::string space_delimiter = " ";
    std::vector<std::string> words;

    size_t pos = 0;
    size_t count = 0;
    while ((pos = argus.find(space_delimiter)) != std::string::npos) {
        words.push_back(argus.substr(0, pos));
        argus.erase(0, pos + space_delimiter.length());
        count++;
    }
    words.push_back(argus);
    if (count != 3)
    {    std::cout << "invalid number of arguments : " << count << std::endl;
        return ;
    }
    
    std::string username = words[0];
    std::string hostname = words[1];
    std::string servername = words[2];
    std::string realname = words[3];
    
    std::cout << "username : <" << username << ">" << std::endl;
    std::cout << "hostname : <" << hostname << ">" << std::endl;
    std::cout << "servername : <" << servername << ">" << std::endl;
    std::cout << "realname : <" << realname << ">" << std::endl;
    
    ircClient->setUsername(username);
    ircClient->setHostname(hostname);
    ircClient->setServername(servername);
    ircClient->setRealname(realname);
    ircClient->setUserFlag();

    if (ircClient->getUserFlag() && ircClient->getPassFlag() && ircClient->getNickFlag())
        ircClient->setRegistFlag();
    
}

void IrcServ::pass_command(IrcClient *ircClient, std::string argus) 
{
    std::cout << "in <PASS> command" << std::endl;
    std::cout << "argus : <" << argus << ">" << std::endl;
    if (argus.find(" ") != std::string::npos) 
        std::cout << "No need SPACE in PASS, find : " << argus.find(" ") << std::endl;
    else
        if (ircClient->checkPASS(argus, getPassword()) == true)
            ircClient->setPassFlag();

    if (ircClient->getUserFlag() && ircClient->getPassFlag() && ircClient->getNickFlag())
        ircClient->setRegistFlag();
               
};



IrcServ::~IrcServ() {};


std::string IrcServ::getPassword()
{
    return _passWord;
}

int IrcServ::GetMaxFd() 
{

    int max_fd_lcl = 0;

    IrcClient *ircClient;
    std::list<IrcClient *>::iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {

        ircClient = *it;
        int commFd = ircClient->getCommFd();
        if (commFd > max_fd_lcl ) {
            max_fd_lcl = commFd;
        }
    }
    return max_fd_lcl;
}


void IrcServ::CopyClientFDtoFDSet(fd_set *fdset) {

    IrcClient *ircClient;
    std::list<IrcClient *>::iterator it;


    for (it = this->_clients.begin(); 
            it != this->_clients.end();
            ++it) {

        ircClient = *it;
        FD_SET(ircClient->getCommFd(), fdset);
    }
}

void IrcServ::RemoveClients(IrcClient *ircClient) {

    this->_clients.remove(ircClient);
    //ircClient객체를 삭제해야하니까 메모리관리 추가??
}

void IrcServ::AddClients(IrcClient *ircClient){

     this->_clients.push_back(ircClient);
}

void IrcServ::part_command(IrcClient *ircClient, std::string argus) 
{
    std::cout << "in <PART> command" << std::endl;
    std::cout << "argus : <" << argus << ">" << std::endl;
    if (argus.find(" ") != std::string::npos) 
        std::cout << "No need SPACE in PART, find : " << argus.find(" ") << std::endl;
    else
        if (ircClient->checkPart(argus) == true)
            ircClient->doPart(argus);               
};

