# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/14 14:53:54 by ehedeman          #+#    #+#              #
#    Updated: 2025/02/14 14:22:38 by ehedeman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRC =		main.cpp	\
			Server.cpp	\
			utils.cpp	\
			Channel.cpp \
			privmsg.cpp \
			join.cpp \
			Commands.cpp \
			topic.cpp \
			mode.cpp

OBJS_DIR = objs/
OBJS = $(SRC:.cpp=.o)
OBJS_PREFIXED = $(addprefix $(OBJS_DIR), $(OBJS))

# client = client

# client_SRC =	Client.cpp	\
# 				Server.cpp

# client_OBJS_DIR = client_objs/
# client_OBJS = $(client_SRC:.cpp=.o)
# client_OBJS_PREFIXED = $(addprefix $(client_OBJS_DIR), $(client_OBJS))

INCLUDES =	Server.hpp Client.hpp Common.hpp Channel.hpp utils.hpp

CXX = c++

RM = rm -rf
MKDIR = mkdir -p
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g -O0 #-fsanitize=address

all: $(NAME) #client

$(NAME): $(OBJS_PREFIXED)
	$(CXX) $(CXXFLAGS) $(OBJS_PREFIXED) -o $(NAME)

$(OBJS_DIR)%.o : %.cpp $(INCLUDES)
	$(MKDIR) $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# client: $(client)

# $(client): $(client_OBJS_PREFIXED)
# 	$(CXX) $(CXXFLAGS) $(client_OBJS_PREFIXED) -o $(client)

# $(client_OBJS_DIR)%.o : %.cpp $(INCLUDES)
# 	$(MKDIR) $(client_OBJS_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) -f $(NAME)

re: fclean all

leaks: all
	valgrind --leak-check=full	\
         --show-leak-kinds=all ./$(NAME)

.PHONY: all clean re fclean leaks