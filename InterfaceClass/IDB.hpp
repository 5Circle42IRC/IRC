#ifndef __IDB_HPP__
#define __IDB_HPP__

#include <map>

template <typename T, typename S>
class IDB : public IClient, public IChannel
{
public:
	virtual const S &find(const T &t) const = 0;
	virtual void deleteTarget(const int targetFd) = 0;
	virtual void addTarget(const int targetFd) = 0;

protected:
	std::map<int, IClient &> _client;
	std::map<std::string, IChannel &> _channel;
};

#endif