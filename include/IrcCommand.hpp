#ifndef __IRCCOMMAND_HPP__
# define __IRCCOMMAND_HPP__

# include "./IrcDB.hpp"
# include <deque>
# include <vector>

//_clientFd 또는 _clientPtr을 멤버변수로 추가하는 건에 대하여...

class IrcCommand {
	public :
		/*db만 받는 생성자는 테스트 이후 삭제해도 됨.*/
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
		void PING();
		void USER();
		void PASS();
		void DISPLAY();
		void KICK();

		IrcCommand& setClientFd(int clientFd);
		std::deque<std::string>& getArgs();
		std::string getCommand();



	private:
		IrcCommand();

		IrcDB* 						_db;
		std::deque<std::string>		_args;
		std::string 				_command;
		int							_clientFd;

		typedef void (IrcCommand::*commandPtr)();
		std::map<std::string, commandPtr> _commandList;

		void joinChannel(std::string name, std::string key);
		int checkValidNICK(std::deque<std::string> args, IrcDB *_db);
		void checkRunCMD();
		void kickUser(std::string channel, std::string name, std::string comment);
		void makeBufferString(IrcChannel *channel, IrcClient *client);

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
        class ERR_NICKNAME_IN_USE: public std::exception {
            virtual const char *what() const throw();
        };
        class ERR_NO_NICKNAME_GIVEN : public std::exception {
            virtual const char *what() const throw();
        };
        class ERR_ERRONEUS_NICKNAME : public std::exception {
            virtual const char *what() const throw();
        };
        //PART
        class ERR_NOT_ON_CHANNEL : public std::exception {
            virtual const char *what() const throw();
        };
        //TOPIC
        class ERR_NEED_MORE_PARAMS : public std::exception {
            virtual const char *what() const throw();
        };
        class ERR_CHAN_OPRIVS_NEEDED : public std::exception {
            virtual const char *what() const throw();
        };
		class ERR_UNKNOWN_MODE : public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_OUT_OF_LIMIT : public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_INVITE_PERSON_ONLY : public std::exception {
			virtual const char *what() const throw();
		};
		class ERR_NOT_OPERATOR : public std::exception {
			virtual const char *what() const throw();
		};
};

#endif