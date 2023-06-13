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


    IrcChannel ch1("testchannel1");
    IrcChannel ch2("testchannel2");

    std::string message_ch = "JOIN #testch";
    commandHandler.parsing(message_ch);
    commandHandler.JOIN(5);
    std::cout << "c1 getbuffer : <" << c1.getBuffer() << ">" << std::endl;
    commandHandler.JOIN(6);
    commandHandler.JOIN(7);

    IrcChannel* testch = db.findChannel("#testch");
    print_users_in_ch(testch);
    std::string message_part = "PART #testch";
    commandHandler.parsing(message_part);
    commandHandler.PART(5);
    std::cout << "after client5 exit #testch" << std::endl;
    print_users_in_ch(testch);

    
/*
    c1.Display();
    std::string message = "NICK ytyys";
    commandHandler.parsing(message);
    commandHandler.NICK(5);
    c1.Display();

    std::string message3 = "NICK nictest sdgsdg";
    c3.Display();
    commandHandler.parsing(message3);
    commandHandler.NICK(7);
    c3.Display();

    std::string message2 = "USER yuser yhost yserver yreal";
    c2.Display();
    commandHandler.parsing(message2);
    commandHandler.USER(6);
    c2.Display();
    
    std::string message4 = "NICK hanmool";
    commandHandler.parsing(message4);
    commandHandler.NICK(6);
    c2.Display();
*/



    return 0;
}