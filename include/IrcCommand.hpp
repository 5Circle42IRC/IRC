#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "./IrcDB.hpp"
# include <deque>
# include <vector>

//_clientFd 또는 _clientPtr을 멤버변수로 추가하는 건에 대하여...

class IrcCommand {
	public :
		/*db만 받는 생성자는 테스트 이후 삭제해도 됨.*/
		IrcCommand(IrcDB *db);
		IrcCommand(IrcDB *db, int clientFd);
		~IrcCommand();

		void parsing(std::string message);
		// void KICK(int clientFd);
		void INVITE();

		void JOIN();
		void NICK();
		// void PASS();
		// void NAME();
		void PRIVMSG();

		void TOPIC();
		void MODE();

		void PART();
		void PONG();
		void USER();

		void DISPLAY();

		IrcCommand& setClientFd(int clientFd);
		std::deque<std::string>& getArgs();
		std::string getCommand();



	private:
		IrcCommand();

		IrcDB* 						_db;
		std::deque<std::string>		_args;
		std::string 				_command;
		int							_clientFd;

		typedef void (IrcCommand::*commandPtrArr)();
		commandPtrArr				_commandPointers[10];
		std::vector<std::string>	_commandNames;
		std::map<std::string, commandPtrArr> _commandList;

		void joinChannel(std::string name, std::string key);
		int checkValidNICK(std::deque<std::string> args, IrcDB *_db);
		void checkRunCMD();

		class ERR_INVALID_ARGUMENT: public std::exception {
			virtual const char *what() const throw();
		};
		
		class ERR_INVALID_PASSWORD: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_USER_ON_CHANNEL: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_INVALID_COMMAND: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_OUT_OF_BOUND_MESSAGE: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_INVALID_NAME_OF_CHANNEL: public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_INVALID_CHAR_IN_NAME: public std::exception {
			virtual const char *what() const throw();
		};
        class ERR_NICKNAMEINUSE: public std::exception {
            virtual const char *what() const throw();
        };
        class ERR_NONICKNAMEGIVEN : public std::exception {
            virtual const char *what() const throw();
        };
        class ERR_ERRONEUSNICKNAME : public std::exception {
            virtual const char *what() const throw();
        };
        //PART
        class ERR_NOTONCHANNEL : public std::exception {
            virtual const char *what() const throw();
        };
        //TOPIC
        class ERR_NEEDMOREPARAMS : public std::exception {
            virtual const char *what() const throw();
        };
        class ERR_CHANOPRIVSNEEDED : public std::exception {
            virtual const char *what() const throw();
        };
		class ERR_UNKNOWNMODE : public std::exception {
			virtual const char *what() const throw();
		};
};

#endif