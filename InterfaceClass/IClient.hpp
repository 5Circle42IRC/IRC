#ifndef __ICLIENT_HPP__
#define __ICLIENT_HPP__

#include <string>

/*
 * 1. canonical form을 지킬 것
 * 2. 생성자에서 반드시 fd, nickname, password 값을 받아서 초기화 할 것
 * 3. fd는 생성시 초기화 이후 변경 금지
 */

class IClient
{
public:
	virtual const int getFd() const = 0;
	virtual const std::string &getNickname() const = 0;
	virtual const std::string &getPassword() const = 0;
	virtual const std::string &getBuffer() const = 0;

	virtual void setNickname(std::string &newNickname) = 0;
	virtual void setPassword(std::string &newPassword) = 0;
	virtual void addBackBuffer(std::string &str) = 0;
	virtual void reduceBuffer(int result) = 0;

protected:
	int _fd;
	std::string _nickname;
	std::string _passWord;
	std::string _buffer;
};
#endif