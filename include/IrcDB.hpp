#ifndef __IRC_DB_HPP__
#define __IRC_DB_HPP__

#include "../InterfaceClass/IDB.hpp"

class DB : private IDB
{
public:
    DB();
    ~DB();
	virtual const IClient& find(const int clientFd) const;
	virtual const IChannel& find(const std::string channelName) const;
	virtual void deleteChannel(const std::string channelName);
	virtual void deleteTargetInChannel(const std::string& channelName, const int targetFd);
	virtual void addTargetInChannel(const std::string& channelName, const int targetFd);
	virtual void registerClient(const int& fd);
	virtual void registerChannel(const std::string& channel);

private:
    const DB& operator=(const DB& copy);
    const DB& DB(const DB& copy);

}

#endif