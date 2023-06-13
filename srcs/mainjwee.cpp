#include "../include/IrcDB.hpp"
#include "../include/IrcClient.hpp"
#include "../include/IrcChannel.hpp"
#include "../include/IrcCommand.hpp"

int main(){
	IrcDB db;
	std::string message = "PRIVMSG juha #123,#123";
	IrcCommand command(&db);

	command.parsing(message);

	std::deque<std::string> ar = command.getArgs();
	
	for (std::deque<std::string>::iterator it = ar.begin(); it != ar.end(); it++)
		std::cout << *it << std::endl;
}