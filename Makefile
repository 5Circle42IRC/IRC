CXX			= c++
CXXFLAGS	= #-Wall -Werror -Wextra -std=c++98 #-g3 -fsanitize=address
TARGET		= ircserv
SRCDIR		= ./srcs
OBJDIR		= ./objs
OBJCMDDIR	= ./objs/command
SRCS		= mainysungwon.cpp \
			  IrcCommand.cpp \
			  IrcClient.cpp \
			  IrcChannel.cpp \
			  IrcDB.cpp \
			  command/NICK.cpp \
			  command/USER.cpp \
			  command/JOIN.cpp
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