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

bool		validFormat(int format, std::string str);
bool		userNameTaken(std::vector<Client> clients, std::string username);
