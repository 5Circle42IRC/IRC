#ifndef __A_CLIENT_HPP__
#define __A_CLIENT_HPP__

#include <string>

class AClient
{
public:
  AClient(const std::string &id, const std::string &_userName, const std::string &realName, const std::string &serverAddress);
  virtual ~AClient();

  const std::string getInputBuf() const;
  const std::string getOnputBuf() const;

  const bool deleteInputBuf(const size_t &len);
  const bool deleteOutputBuf(const size_t &len);

  const bool addBackInputBuf(const std::string &str);
  const bool addBackOutputBuf(const std::string &str);

  bool setNickname(const std::string &nickname);

  const std::string &getNickname() const;
  const std::string &getUserName() const;
  const std::string &getRealName() const;
  const std::string &getServerAddress() const;

protected:
  std::string _nickname; // id
  std::string _userName; // serverName;
  std::string _realName; // optional
  std::string _serverAddress;

  std::string _inputBuffer;
  std::string _outputBuffer;

private:
  AClient();
  AClient(const AClient &copy);
  const AClient &operator=(const AClient &copy);
};

#endif
