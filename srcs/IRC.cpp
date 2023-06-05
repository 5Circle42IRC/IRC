#include "../include/Irc.hpp"

void ErrorHandle::errorHandle(std::string message, int errorNum)
{
    std::cout << message << std::endl;
    exit(errorNum);
}
