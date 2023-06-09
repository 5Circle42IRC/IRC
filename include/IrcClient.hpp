#ifndef __IrcCLIENT__
#define __IrcCLIENT__

#include <string>

class IrcClient
{
public:
    IrcClient();
    IrcClient(int fd, std::string nickname, std::string password, std::string buffer);
    ~IrcClient();

    int getFd() const;
    const std::string &getNickname() const;
    const std::string &getPassword() const;
    const std::string &getBuffer() const;

    void setNickname(std::string &newNickname);
    void setUsername(std::string &newUsername);
    void setHostname(std::string &newHostname);
    void setServername(std::string &newServername);
    void setRealname(std::string &newRealname);
    void setPassword(std::string &newPassword);
    void addBackBuffer(std::string &str);
    void reduceBuffer(int result);
    IrcClient(const IrcClient &copy);
    const IrcClient &operator=(const IrcClient &copy);

protected:
	int         _fd;
	std::string _nickname;
	std::string _password;
	std::string _buffer;

private :

    
};

#endif