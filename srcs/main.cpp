#include "../include/IRC.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
        ErrorHandle::errorHandle("Usage : ./ircserv [port] [password]", 0);
    (void)argc;

    char *isError = NULL;
    long port = std::strtol(argv[1], &isError, 10);

    if (*isError || (0 > port || 65535 < port))
        ErrorHandle::errorHandle("port error", -1);

    IrcServ serv(port, argv[2]);
    serv.run();
/*
    printf("port : %d\n", port);
    printf("ip : %d\n", htonl(INADDR_ANY));
*/
    return 0;
}