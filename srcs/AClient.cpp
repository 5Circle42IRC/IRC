#include "../abstractClass/AClient.hpp"

AClient::AClient(const std::string &id, const std::string &_userName, const std::string &realName, const std::string &serverAddress)
    : _nickname(id), _userName(_userName), _realName(realName), _serverAddress(serverAddress){};
AClient::~AClient(){};

const std::string AClient::getInputBuf() const
{
  return _inputBuffer;
};

const std::string AClient::getOnputBuf() const
{
  return _outputBuffer;
};

const bool AClient::deleteInputBuf(const size_t &len)
{
  if (len > _inputBuffer.size())
    return false;
  _inputBuffer.erase(0, len);
  return true;
};

const bool AClient::deleteOutputBuf(const size_t &len)
{
  if (len > _outputBuffer.size())
    return false;
  _outputBuffer.erase(0, len);
  return true;
};

const bool AClient::addBackInputBuf(const std::string &str)
{
  _inputBuffer += str;
  return true;
};

const bool AClient::addBackOutputBuf(const std::string &str)
{
  _outputBuffer += str;
  return true;
};

bool AClient::setNickname(const std::string &nickname)
{
  _nickname = nickname;
  return true;
};

const std::string &AClient::getNickname() const
{
  return _nickname;
};

const std::string &AClient::getUserName() const
{
  return _userName;
};

const std::string &AClient::getRealName() const
{
  return _realName;
};

const std::string &AClient::getServerAddress() const
{
  return _serverAddress;
};