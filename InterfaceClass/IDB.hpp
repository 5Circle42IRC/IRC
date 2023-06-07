#ifndef __IDB_HPP__
#define __IDB_HPP__

#include <map>



class IClient;
class IChannel;

template <typename T, typename S>
class IDB : public IClient, public IChannel
{
public:
    virtual const S&	find(const T& t) const = 0;
	virtual void		deleteTarget(const int targetFd) = 0;

protected:
	std::map <int, IClient&>          _clinet;
	std::map <std::string, IChannel&> _channel;
};

#endif