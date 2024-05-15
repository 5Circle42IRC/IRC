#ifndef __CLIENT_LIST_HPP__
#define __CLIENT_LIST_HPP__

#include "../abstractClass/AClientList.hpp"

class ClientList : public AClientList
{
public:
  ClientList();
  virtual ~ClientList();

private:
  ClientList(const ClientList &copy);
  const ClientList &operator=(const ClientList &copy);
};

#endif
