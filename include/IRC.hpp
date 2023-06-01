#ifndef __IRC_hpp__
#define __IRC_hpp__

#include <cstdlib>

#include <string>
#include <iostream>

#include <algorithm>
#include <vector>
#include <list>
#include <map>

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "IrcChannel.hpp"
#include "IrcClient.hpp"
#include "IrcServ.hpp"

class ErrorHandle 
{
public:
    static void errorHandle(std::string message, int errorNum);
private:
    ~ErrorHandle();
    ErrorHandle(std::string message, int errorNum);
    const ErrorHandle& operator=(const ErrorHandle& copy);
    ErrorHandle();
    ErrorHandle(const ErrorHandle& errorHandle);
};

#endif