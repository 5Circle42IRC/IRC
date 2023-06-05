CXX			= c++
CXXFLAGS	= -Wall -Werror -Wextra -std=c++98 -g3 -fsanitize=address
TARGET		= ircserv
OBJS		= $(SRCS:.cpp=.o)
SRCS 		= ./srcs/main.cpp 		\
			  ./srcs/Irc.cpp        \
			  ./srcs/IrcServ.cpp	\
			  ./srcs/IrcClient.cpp	\
			  ./srcs/IrcChannel.cpp	\

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) 

.PHONY : all clean fclean re

%.o : %.c
	$(CXX) $(CXXFLAGS) $^ -c $(TARGET) 

all : 
	$(TARGET)

clean : 
	@rm -f $(OBJS)

fclean : 
	@make clean
	@rm -f $(TARGET)
	@rm -rf *.dSYM

re :
	@make fclean
	@make -j4