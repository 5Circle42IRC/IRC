#include "../include/IrcDB.hpp"
#include "../include/IrcClient.hpp"
#include "../include/IrcChannel.hpp"
#include "../include/IrcCommand.hpp"

int main(){
	IrcDB db;
	std::string message = "NICK juha";
	IrcClient *juha = new IrcClient(4, "juha", "1234", "");
	IrcCommand command(&db);

	command.parsing(message);
	std::cout << command.getCommand() << std::endl;
	std::cout << command.getArgs()[0] << std::endl;



}