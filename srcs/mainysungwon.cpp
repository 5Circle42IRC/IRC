#include "../include/IrcServ.hpp"
#include "../include/IrcClient.hpp"
#include "../include/IrcChannel.hpp"
#include "../include/IrcDB.hpp"
#include "../include/IrcCommand.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

static void errorHandle(std::string message, int exitCode)
{
    std::cout << message << std::endl;
    exit(exitCode);
}

int main(int argc, char **argv)
{
    
    IrcDB      db;
    IrcCommand commandHandler(&db);
    std::string message = "NICK ytyy";

    commandHandler.parsing(message);
    IrcClient c1(5, "ysungwon1", "password", "");
    IrcClient c2(6, "ysungwon2", "password", "");
    IrcClient c3(7, "ysungwon3", "password", "");
    IrcClient c4(8, "ysungwon4", "password", "");
    IrcClient c5(9, "ysungwon5", "password", "");


    c1.Display();
    
    db.insertClient(&c1);
    db.insertClient(&c2);
    db.insertClient(&c3);
    db.insertClient(&c4);
    db.insertClient(&c5);

    commandHandler.NICK(5);

    c1.Display();

    return 0;
}