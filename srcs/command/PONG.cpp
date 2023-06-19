#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"

void IrcCommand::PING(){
    IrcClient* client = _db->findClientByFd(_clientFd);
    client->addBackBuffer(client->getNickname() + " PONG" + "\r\n");
}