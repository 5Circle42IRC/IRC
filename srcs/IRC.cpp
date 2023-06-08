#include "../include/Irc.hpp"

void Function::errorHandle(std::string message, int errorNum)
{
    std::cout << message << std::endl;
    exit(errorNum);
}

const char* CopyError::what() const throw()
{
    return ("CopyError");
}