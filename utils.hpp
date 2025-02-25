#pragma once
#include <iostream>
#include <cstring>
#include "Server.hpp"

typedef enum	s_format
{
	NICKNAME,
	USERNAME,
	PASSWORD,
	PORT

}				t_format;

bool			validFormat(int format, std::string str);
bool			userNameTaken(std::vector<Client> clients, std::string username);
std::string		removeNewline(char *buff);
bool			verfifyPassword(std::string password, int clientSocket);
std::string		requestName(int format, int clientSocket, std::vector<Client> &clients);


/* join */
void			join(Server *server, Client *joiningClient, std::string channelTojoin);

/* privmsg */
void			privmsg(Server *server, Client *sender, std::string cmd);
