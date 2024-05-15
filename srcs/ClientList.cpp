#include "../include/ClientList.hpp"

ClientList::ClientList()
    : AClientList() {}

ClientList::~ClientList()
{
  for (int i = 0; i < 1024; i++)
  {
    if (_pointerClient[i] != NULL)
      delete _pointerClient[i];
  }
};
