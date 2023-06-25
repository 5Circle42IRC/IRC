# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysungwon <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/21 18:45:57 by ysungwon          #+#    #+#              #
#    Updated: 2023/06/21 18:46:00 by ysungwon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			= c++
CXXFLAGS	= -g3 -Wall -Werror -Wextra -std=c++98  -fsanitize=address
TARGET		= ircserv
SRCDIR		= ./srcs
OBJDIR		= ./objs
OBJCMDDIR	= ./objs/command
SRCS		= main.cpp \
			  IrcCommand.cpp \
			  IrcClient.cpp \
			  IrcChannel.cpp \
			  IrcDB.cpp \
			  IrcServ.cpp \
			  command/NICK.cpp \
			  command/USER.cpp \
			  command/JOIN.cpp \
			  command/PART.cpp \
			  command/PRIVMSG.cpp \
			  command/TOPIC.cpp \
			  command/INVITE.cpp \
			  command/PONG.cpp \
			  command/MODE.cpp\
			  command/DISPLAY.cpp \
			  command/KICK.cpp \
			  command/PASS.cpp
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
