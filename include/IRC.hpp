#ifndef __IRC_hpp__
#define __IRC_hpp__

#define BUFFER_SIZE 1024

#include <cstdlib>

#include <string>
#include <iostream>

#include <algorithm>
#include <vector>
#include <map>

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "IrcChannel.hpp"
#include "IrcClient.hpp"
#include "IrcServ.hpp"
#include "IrcCommand.hpp"

class Function 
{
public:
    static void errorHandle(std::string message, int errorNum);
private:
    ~Function();
    Function(std::string message, int errorNum);
    const Function& operator=(const Function& copy);
    Function();
    Function(const Function& errorHandle);
};

class CopyError : public std::exception
{
public:
    virtual const char* what() const throw();
};

#endif