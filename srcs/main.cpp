#include "../include/IrcServ.hpp"

static void errorHandle(std::string message, int exitCode)
{
    std::cout << message << std::endl;
    exit(exitCode);
}

int main(int argc, char **argv)
{
    if (argc != 3)
        errorHandle("Usage : ./ircserv [port] [password]", 0);
    (void)argc;

    char *isError = NULL;
    long port = std::strtol(argv[1], &isError, 10);

    if (*isError || (0 > port || 65535 < port))
        errorHandle("port error", -1);

    try {
        IrcServ serv(port, argv[2]);
        serv.run();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "unkown exception" << std::endl;
    }
/*
    printf("port : %d\n", port);
    printf("ip : %d\n", htonl(INADDR_ANY));
*/
    return 0;
}