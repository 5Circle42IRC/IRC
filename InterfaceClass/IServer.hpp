#ifndef __ISERVER_HPP__
# define __ISERVER_HPP__

# include <iostream>
# include <cstdlib>

# include <string>
# include <iostream>

# include <map>

# include <sys/socket.h>
# include <sys/select.h>
# include <netinet/in.h>
# include <netinet/ip.h>

# include "./ICommand.hpp"

/*
 * 1. canonical form을 지킬 것
 * 2. 상속관계를 고려하여 변수 및 함수 정리 필요
 * 3. write랑 여러가지 것들을 추가할 필요가 있음.
 * 4. protocal을 만드는 것을 고려. (기능 분리)
*/

class IServer: public ICommand {
	public:
		virtual void	run() = 0;
		virtual int		getMaxFd() = 0;
		virtual	void	CopyClientFDtoFDSet(fd_set *fdset) = 0;
    	virtual void 	RemoveClients(IrcClient *ircClient) = 0;
    	virtual void 	AddClients(IrcClient *ircClient) = 0;

		virtual Command Parse(std::string& message) = 0;

		virtual void	addClient(int acceptFd) = 0;
		virtual void	readClient() = 0;
	
	protected:
		bool		_isError;

		int			_port;
		std::string	_passWord;

		int			_servFd;
		int			_fdMax;
		int			_fdNum;
		int			_opt;

		fd_set		_activeReads;
		fd_set		_activeWrite;
		fd_set		_cpyReads;
		fd_set		_cpyWrites;

		sockaddr_in							_servAddr;
		sockaddr_in                         _servAddr;
		socklen_t                           _servAddrLen;
		std::map<int, IrcClient>            _clients;
		std::map<std::string, IrcChannel>   _channels;

		struct timeval _timeout;

        char               _message[BUFFER_SIZE];
        int                _readLen;
        struct sockaddr_in _clientAddr;
};

#endif