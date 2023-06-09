#ifndef __IrcCLIENT__
#define __IrcCLIENT__

class IrcClient
{
public:
    IrcClient();
    IrcClient(int fd, std::string nickname, std::string password, std::string buffer);
    ~IrcClient();

    virtual const int getFd() const;
    virtual const std::string &getNickname() const;
    virtual const std::string &getPassword() const;
    virtual const std::string &getBuffer() const;

    virtual void setNickname(std::string &newNickname);
    virtual void setPassword(std::string &newPassword);
    virtual void addBackBuffer(std::string &str);
    virtual void reduceBuffer(int result);
    

protected:
	int         _fd;
	std::string _nickname;
	std::string _password;
	std::string _buffer;

private :
    IrcClient(const IrcClient &copy);
    const IrcClient &operator=(const IrcClient &copy);
    
};

#endif