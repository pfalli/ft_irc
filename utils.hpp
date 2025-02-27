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
void	join(Server *server, Client *joiningClient, std::string channelTojoin);
int		isChannleExist(std::vector<Channel>&	channels, Client *joiningClient, std::string tmpChannelTojoin);

/* privmsg */
void	privmsg(Server *server, Client *sender, const Command &cmd);
void	signal_handler(int signal);
void	parseChannelName2(std::string &targetChannel);

template <typename T> typename std::vector<T>::iterator		findObject(int toFind, std::vector<T> &array);