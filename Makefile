# CXX			= c++
# CXXFLAGS	= -Wall -Werror -Wextra -std=c++98 -g3 -fsanitize=address
# TARGET		= ircserv
# OBJS		= $(SRCS:.cpp=.o)
# SRCS 		= ./srcs/main.cpp 		\
# 			  ./srcs/IrcServ.cpp	\
# 			  ./srcs/IrcClient.cpp	\
# 			  ./srcs/IrcChannel.cpp	\
# 			  ./srcs/command/IrcCommand.cpp \
# 			  ./srcs/command/NICK.cpp

# $(TARGET) : $(OBJS)
# 	$(CXX) $(CXXFLAGS) $^ -o $@

# .PHONY : all clean fclean re

# %.o : %.cpp
# 	$(CXX) $(CXXFLAGS) $< -c $@

# all : 
# 	$(TARGET)

# clean : 
# 	@rm -f $(OBJS)

# fclean : 
# 	@make clean
# 	@rm -f $(TARGET)
# 	@rm -rf *.dSYM

# re :
# 	@make fclean
# 	@make -j4

CXX			= c++
CXXFLAGS	= -Wall -Werror -Wextra -std=c++98 -g3 -fsanitize=address
TARGET		= ircserv
SRCDIR		= ./srcs
OBJDIR		= ./objs
OBJCMDDIR	= ./objs/command
SRCS		= main.cpp \
			  IrcServ.cpp \

OBJS		= $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJCMDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean fclean re

all: $(TARGET)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(TARGET)

re: fclean
	@make -j4 all