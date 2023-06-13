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
void print_users_in_ch(IrcChannel* testch)
{
    std::cout << "in printuserinch fn " << std::endl;
    std::map<int, bool> users = testch->getUser();
    std::map<int, bool>::iterator it;
    for (it = users.begin();
        it != users.end();
        it++)    
        {
            std::cout << "it->first : <" << it->first << ">" << std::endl;
        }
}

int main(int argc, char **argv)
{
    try {
            
        IrcDB      db;
        IrcCommand commandHandler(&db);

        IrcClient c1(5, "ysungwon1", "password", "");
        IrcClient c2(6, "ysungwon2", "password", "");
        IrcClient c3(7, "ysungwon3", "password", "");
        IrcClient c4(8, "ysungwon4", "password", "");
        IrcClient c5(9, "ysungwon5", "password", "");
        db.insertClient(&c1);
        db.insertClient(&c2);
        db.insertClient(&c3);
        db.insertClient(&c4);
        db.insertClient(&c5);
        std::cout << "c2.buffer : <" << c2.getBuffer() << ">" << std::endl;
        std::string message_ch = "JOIN #testch";
        commandHandler.parsing(message_ch);
        commandHandler.JOIN(5);
        commandHandler.JOIN(6);
        commandHandler.JOIN(7);
        std::cout << "c2.buffer : <" << c2.getBuffer() << ">" << std::endl;

    std::string message_PRIVMSG2 = "PRIVMSG ysungwon1 testmsg";
    c2.Display();
    commandHandler.parsing(message_PRIVMSG2);
    commandHandler.PRIVMSG(6);
    std::cout << "c2.buffer : <" << c2.getBuffer() << ">" << std::endl;
    c2.Display();    
    }
   catch (const std::exception& e)
    {
        //std::fprintf(stderr, "Error: %s\n", e.what());
        std::cout << "in main Exception : < " << e.what() << ">" << std::endl;
        return 0;
    }
    return 0;
}