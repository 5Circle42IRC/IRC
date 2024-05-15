#ifndef __A_CLIENT_LIST_HPP__
#define __A_CLIENT_LIST_HPP__

#include "AClient.hpp"

class AClientList
{
public:
  AClientList();
  virtual ~AClientList();

  const bool exsistClient(int &fd);
  const bool removeClient(int &fd);
  const bool registerClient(int fd, const std::string &id, const std::string &_userName, const std::string &realName, const std::string &serverAddress);

  const AClient *getClient(const int &fd);
  const AClient *getClient(const std::string &nickname);

protected:
  AClient *_pointerClient[1024];

private:
  AClientList(const AClientList &copy);
  const AClientList &operator=(const AClientList &copy);
};

#endif
