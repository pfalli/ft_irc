# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/14 14:53:54 by ehedeman          #+#    #+#              #
#    Updated: 2025/02/13 12:48:59 by ehedeman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

server = server

Server_SRC =	main.cpp	\
				Server.cpp

Server_OBJS_DIR = server_objs/
Server_OBJS = $(Server_SRC:.cpp=.o)
Server_OBJS_PREFIXED = $(addprefix $(Server_OBJS_DIR), $(Server_OBJS))

client = client

client_SRC =	Client.cpp	\
				Server.cpp

client_OBJS_DIR = client_objs/
client_OBJS = $(client_SRC:.cpp=.o)
client_OBJS_PREFIXED = $(addprefix $(client_OBJS_DIR), $(client_OBJS))

INCLUDES =	Server.hpp

CXX = c++

RM = rm -rf
MKDIR = mkdir -p
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

all: server client

server: $(server)

$(server): $(Server_OBJS_PREFIXED)
	$(CXX) $(CXXFLAGS) $(Server_OBJS_PREFIXED) -o $(server)

$(Server_OBJS_DIR)%.o : %.cpp $(INCLUDES)
	$(MKDIR) $(Server_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

client: $(client)

$(client): $(client_OBJS_PREFIXED)
	$(CXX) $(CXXFLAGS) $(client_OBJS_PREFIXED) -o $(client)

$(client_OBJS_DIR)%.o : %.cpp $(INCLUDES)
	$(MKDIR) $(client_OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(Server_OBJS_DIR)

fclean: clean
	$(RM) -f $(server)

re: fclean all

leaks: all
	valgrind --leak-check=full	\
         --show-leak-kinds=all ./$(server)

.PHONY: all clean re fclean leaks