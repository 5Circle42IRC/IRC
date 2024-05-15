#include "../abstractClass/AClientList.hpp"

AClientList::AClientList()
    : _pointerClient{NULL} {}

AClientList::~AClientList()
{
  for (int i = 0; i < 1024; i++)
  {
    if (_pointerClient[i] != NULL)
      delete _pointerClient[i];
  }
};

const bool AClientList::exsistClient(int &fd)
{
  if (_pointerClient[fd] == NULL)
    return false;
  return true;
};

const bool AClientList::removeClient(int &fd)
{
  if (_pointerClient[fd] == NULL)
    return false;
  delete _pointerClient[fd];
  _pointerClient[fd] = NULL;
  return true;
};

const bool AClientList::registerClient(int fd, const std::string &id, const std::string &_userName, const std::string &realName, const std::string &serverAddress)
{
  if (_pointerClient[fd] != NULL)
    return false;
  _pointerClient[fd] = new AClient(id, _userName, realName, serverAddress);
  return true;
};

const AClient *AClientList::getClient(const int &fd)
{
  return _pointerClient[fd];
};

const AClient *AClientList::getClient(const std::string &nickname)
{
  for (int i = 0; i < 1024; i++)
  {
    if (_pointerClient[i] != NULL && _pointerClient[i]->getNickname() == nickname)
      return _pointerClient[i];
  }
  return NULL;
};