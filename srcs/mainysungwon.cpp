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

        IrcChannel ircChannel("#123");
        db.insertChannel(&ircChannel);
        db.findChannel("#123")->addUser(5);
        db.findChannel("#123")->addUser(6);
        db.findChannel("#123")->addUser(7);
        db.findChannel("#123")->addUser(8);
        db.findChannel("#123")->addUser(9);

        std::string message_ch = "INVITE ysungwon2 #123";
        commandHandler.parsing(message_ch);
        commandHandler.INVITE(5);
        
        message_ch = "INVITE ysungwon76 #123";
        commandHandler.INVITE(7);
        commandHandler.INVITE(9);

        message_ch = "INVITE ysungwon5 #123123";
        commandHandler.INVITE(7);
/*
        ////////////////PART test/////////////////



        std::cout << "c1 getbuffer : <" << c1.getBuffer() << ">" << std::endl;
        std::cout << "find #testch" << std::endl;
        IrcChannel* testch = db.findChannel("#testch");
        std::cout << " print user in ch testch" << std::endl;
        print_users_in_ch(testch);
        std::string message_part = "PART #testch";
        commandHandler.parsing(message_part);
        commandHandler.PART(5);
        std::cout << "after client5 exit #testch" << std::endl;
        print_users_in_ch(testch);
        std::cout << "not registered client call part" << std::endl;
        commandHandler.PART(8);

        std::string message_part2 = "PART testch";
        commandHandler.parsing(message_part2);    
        std::cout << "wrong part name" << std::endl;
        commandHandler.PART(9);

        std::string message_part3 = "PART #";
        commandHandler.parsing(message_part3);    
        std::cout << "only #" << std::endl;
        commandHandler.PART(9);    
*/

/*   
        ////////////////////////NICK test//////////////////////

        ////same nick test/////
        c1.Display();
        std::string message = "NICK ysungwon2";
        commandHandler.parsing(message);
        commandHandler.NICK(5);
        c1.Display();        
 
        
        ////nick working test///
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
    */
    return 0;
}