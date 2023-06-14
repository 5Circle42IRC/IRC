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
void print_users_in_ch_name(IrcDB *db, std::string name)
{
    IrcChannel *testch;
    testch = db->findChannel(name);
    std::cout << "in printuserinch fn " << std::endl;
    std::map<int, bool> users = testch->getUser();
    std::map<int, bool>::iterator it;
    for (it = users.begin();
        it != users.end();
        it++)    
        {
            std::cout << "it->first : <" << it->first << "> , < " << it->second << " >" << std::endl;
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
        
        db.insertClient(&c1);
        db.insertClient(&c2);
        std::string message = "JOIN #123 1234\nJOIN #abc 1234";
       commandHandler.setClientFd(5).parsing(message);
    //    std::string message2 = "JOIN #123 1234\nJOIN #abc abcd";
    //    commandHandler.setClientFd(6).parsing(message);
    //    print_users_in_ch_name(&db, "#123");
    //    print_users_in_ch_name(&db, "#abc");
    } catch (std::exception &e){
        std::cout << e.what() << std::endl;
    }
    return 0;
}