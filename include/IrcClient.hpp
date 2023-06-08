#ifndef __IrcCLIENT__
#define __IrcCLIENT__

#include "../InterfaceClass/IClient.hpp"

class IrcClient : private IClient
{
public:
    IrcClient();
    IrcClient(int fd, std::string nickname, std::string passWord, std::string buffer);
    ~IrcClient();

    virtual const int getFd() const;
    virtual const std::string &getNickname() const;
    virtual const std::string &getPassword() const;
    virtual const std::string &getBuffer() const;

    virtual void setNickname(std::string &newNickname);
    virtual void setPassword(std::string &newPassword);
    virtual void addBackBuffer(std::string &str);
    virtual void reduceBuffer(int result);

private :
    IrcClient(const IrcClient &copy);
    const IrcClient &operator=(const IrcClient &copy);
};

#endif