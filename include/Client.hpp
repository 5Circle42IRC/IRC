#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <string>
#include "../abstractClass/AClient.hpp"

class Client : public AClient
{
public:
  Client(const std::string &id, const std::string &_userName, const std::string &realName, const std::string &serverAddress);
  virtual ~Client();

private:
  Client();
  Client(const Client &copy);
  const Client &operator=(const Client &copy);
};

#endif
