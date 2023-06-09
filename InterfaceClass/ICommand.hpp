#ifndef __ICOMMAND_HPP__
# define __ICOMMAND_HPP__

# include <iostream>
# include <vector>
# include <map>
# include "./IDB.hpp"

/*
 * 1. canonical form을 지킬 것
 * 2. foreach(T)에는 client의 map 또는 channel의 map이 인자로 들어가서 순회함
 * 3. execute는 각 클래스에서 생성된 객체가 업캐스팅 되어 호출함
*/


class ICommand: public IDB{
	public:
		virtual void	foreach(std::map<int, IClient&>& client) = 0;
		virtual void	foreach(std::map<std::string, IChannel&> channel) = 0;
		virtual void	execute(int ClientFd, std::vector<std::string>args) = 0;

		virtual const	std::vector<std::string>& getArgs() const = 0;
		virtual void	setArgs() = 0;
};

#endif