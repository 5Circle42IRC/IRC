#include "../include/IrcServ.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

static void errorHandle(std::string message, int exitCode)
{
    std::cout << message << std::endl;
    exit(exitCode);
}

int main(int argc, char **argv)
{
    if (argc != 3)
        errorHandle("Usage : ./ircserv [port] [password]", 0);

    char *isError = NULL;
    long port = std::strtol(argv[1], &isError, 10);
    int hostFd;

    if (*isError || (0 > port || 65535 < port))
        errorHandle("port error", -1);
    try {
        IrcServ serv(port, argv[2]);
        hostFd = serv.on();
        if (false == serv.setSelect())
        {
            // handling
        }
        while (42)
        {
            serv.setSelect();

        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "unkown exception" << std::endl;
    }
    close(hostFd);
    return 0;
}

/*
    printf("port : %d\n", port);
    printf("ip : %d\n", htonl(INADDR_ANY));
*/