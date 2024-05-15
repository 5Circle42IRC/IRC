#include "../include/Client.hpp"

Client::Client(const std::string &id, const std::string &_userName, const std::string &realName, const std::string &serverAddress)
    : AClient(id, _userName, realName, serverAddress){};
Client::~Client(){};