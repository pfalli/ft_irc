#include "utils.hpp"

class PasswordFormatException : public std::exception
{
	public:
		const char*						what() const throw()
		{
			const char *ret = "Error: invalid format: password.\nPlease make "\
				"sure the password is according to the format.\n(between 8 and"\
				" 20 characters, must be alpha-numeric or special character such as _-=+!)";
			return (ret);
		}
};

class PortFormatException : public std::exception
{
	public:
		const char*						what() const throw()
		{
			const char *ret = "Error: invalid format: port.\nPlease make "\
				"sure the port is according to the format.\n(5 or less numbers)";
			return (ret);
		}
};


bool		userNameTaken(std::vector<Client> clients, std::string username)
{
	std::vector<Client>::iterator it = clients.begin();
	while (it != clients.end())
	{
		if ((*it).getUserName() == username)
			return true;
		it++;
	}
	return false;
}

static bool validUsername(std::string str)
{
	if (strlen(str.c_str()) > 30 || strlen(str.c_str()) < 4)
		return (false);
	for (size_t i = 0; i < strlen(str.c_str()); i++)
	{
		if (!isalnum(str[i]) && !(str[i] <= 40 && str[i] >= 21))
			return (false);
	}
	return (true);
}

static bool validNickname(std::string str)
{
	if (strlen(str.c_str()) > 30 || strlen(str.c_str()) < 4)
		return (false);
	for (size_t i = 0; i < strlen(str.c_str()); i++)
	{
		if (!isalnum(str[i])  && !(str[i] <= 40 && str[i] >= 21))
			return (false);
	}
	return (true);
}
static bool validPassword(std::string str)
{
	if (strlen(str.c_str()) > 20 || strlen(str.c_str()) < 2)
		return (false);
	for (size_t i = 0; i < strlen(str.c_str()); i++)
	{
		if (!isalnum(str[i]) && !(str[i] <= 40 && str[i] >= 21) )
			return (false);
	}
	return (true);
}
static bool validPort(std::string str)
{
	if (strlen(str.c_str()) > 5)
		return false;
	for (size_t i = 0; i < strlen(str.c_str()); i++)
	{
		if (!isdigit(str[i]))
			return false;
	}
	return (true);
}


bool validFormat(int format, std::string str)
{
	if (format == USERNAME)
	{
		if (validUsername(str))
			return true;
	}
	else if (format == NICKNAME)
	{
		if (validNickname(str))
			return true;
	}
	else if (format == PASSWORD)
	{
		if (!validPassword(str))
			throw PasswordFormatException();
	}
	else if (format == PORT)
	{
		if (!validPort(str))
			throw PortFormatException();
	}
	return false;
}
// std::string					removeNewline(char *buff) // *** it was deleting '#', ':', and more chars ***
// {
// 	std::string _new = "";

// 	for (size_t i = 0; i < strlen(buff); i++)
// 	{
// 		if (!isalnum(buff[i]) && buff[i] != ' ')
// 			break;
// 		_new += buff[i];
// 	}
// 	return _new;
// }

std::string removeNewline(char *buff) // ***good one***
{
	std::string _new = "";

	for (size_t i = 0; i < strlen(buff); i++)
	{
		if (buff[i] != '\n' && buff[i] != '\r')
		{
			_new += buff[i];
		}
	}
	return _new;
}


bool	verfifyPassword(std::string password, int clientSocket)
{
	std::string from_client;
	char buff[1024] = {0};
	int i;
	for (i = 0; i < 3; i++)	
	{
		const char *pw_check = "Please enter the password:\n";
		send(clientSocket, pw_check, strlen(pw_check), 0);
		recv(clientSocket, buff, sizeof(buff), 0);
		from_client = removeNewline(buff);
		if (from_client == password)
			break ;
		std::string temp = "Wrong Password.\n";
		const char *to_client = temp.c_str();
		send(clientSocket, to_client, strlen(to_client), 0);
	}
	if (i == 3)
	{
		close (clientSocket);
		return (false);
	}
	return true;
}

std::string					requestName(int format, int clientSocket, std::vector<Client> &clients)
{
	char buff[1024] = {0};
	std::string _name;
	const char *to_client;
	while (true)
	{
		if (format == USERNAME)
			to_client = "Please enter your user name (user name != nickname):\n";
		else if (format == NICKNAME)
			to_client = "Please enter your nick name (user name != nickname):\n";
		send(clientSocket, to_client, strlen(to_client), 0);
		recv(clientSocket, buff, sizeof(buff), 0);
		_name = removeNewline(buff);
		if (validFormat(format, _name))
		{
			if (!userNameTaken(clients, _name))
				break;
			else
			{
				to_client = "Username already taken.\n";
				send(clientSocket, to_client, strlen(to_client), 0);
			}
		}
		else
		{
			to_client = "Wrong Format.\n";
			send(clientSocket, to_client, strlen(to_client), 0);
		}
	}
	return (_name);
}


void	signal_handler(int signal)
{
	signal++;

	Server &server = static_cast<Server &>(*global_server);
	server.setServerShutdown();
}

std::string timeToString(time_t t)
{
	std::ostringstream oss;
	oss << t;
	return oss.str();
}

size_t stringToSizeT(std::string &str)
{
	size_t result;
	std::istringstream iss(str);
	iss >> result;
	if (iss.fail() || !iss.eof())
		return (0);
	return (result);
}

std::string convertSizeTtoString(size_t number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

bool isOperator(Channel &channel, Client *client)
{
	std::vector<Client *>::iterator it = channel.getOperators().begin();
	for (size_t i = 0; i < channel.getOperators().size(); i++)
	{
		if ((*it)->getNickName() == client->getNickName())
			break ;
		it++;
	}
	if (it == channel.getOperators().end())
		return false;
	return true ;
}

std::string splitParamsName(std::string params, int mode)
{
	std::string name = "";
	if (mode == USERNAME)
	{
		size_t i = 0;
		while (i < params.length() && params[i] != ' ')
		{
			name += params[i];
			i++;
		}
		// if (!params[i])
		// 	return (name);
		// else
		// {
		// 	if (isspace(params[i]))
		// 	{
		// 		i++;
		// 		if (params[i] && params[i] == '0')
		// 		{
		// 			std::cout << params[i] << std::endl << std::endl;
		// 			i++;
		// 			if (params[i] && isspace(params[i]))
		// 			{
		// 				std::cout << params[i] << std::endl << std::endl;
		// 				i++;
		// 				if (params[i] && params[i] == '*')
		// 				{
		// 					std::cout << params[i] << std::endl << std::endl;
		// 					i++;
		// 					if ((params[i] && isspace(params[i])))
		// 						return (name);
		// 					else
		// 						return ("ERROR");
		// 				}
		// 				else
		// 					return ("ERROR");
		// 			}
		// 			else
		// 				return ("ERROR"); 
		// 		}
		// 		else
		// 			return ("ERROR");
		// 	}
		// 	else
		// 		return ("ERROR");
		// }
	}
	else if (mode == REALNAME)
	{
		size_t i = 0;
		while (i < params.length() && params[i] != '*')
		{
			std::cout << params[i];
			i++;
		}
		if (params[i])	//skip whitespace after *
			i++;
		else
			return ("not_given");
		while (i < params.length())
		{
			name += params[i];
			i++;
		}
	}
	return (name);
}

bool	checkCaseHex(const Command &cmd, Client & client)
{
	if (cmd.command == "NICK" && !cmd.message.empty())
	{
		std::string param = cmd.parameter;
		const char *ptr = strstr(param.c_str(), "USER");
		if (!ptr)
			return false;
		std::string nickname = "";
		size_t i;
		for (i = 0; i < param.length(); i++)
		{
			if (isspace(param[i]))
			{
				i++;
				if (ptr == &param[i])
					break ;
			}
			nickname += param[i];
		}
		client.setNickName(nickname);
		if (&param[i] == ptr)
		{
			i += 5;	//skip whitespace after USER
			std::string username = "";
			while (i < param.length())
			{
				if (isspace(param[i]))
					break;
				username += param[i];
				i++;
			}
			client.setUserName(username);
			client.setRealName(cmd.message);
			client.setRegistered();
			send(client.getSocket(), "Registration complete. Now you just need the password.\r\n", 57, 0);
			return true;
		}
	}
	else if (cmd.command == "CAP")
	{
		std::string param = cmd.parameter;
		const char *nick_ptr = strstr(param.c_str(), "NICK");
		if (!nick_ptr)
			return false;
		const char *user_ptr = strstr(param.c_str(), "USER");
		if (!user_ptr)
			return false;
		size_t i;
		for (i = 0; i < param.length(); i++)
		{
			if (&param[i] == nick_ptr)
			{
				i += 5;	//5 to skip whitespace after too
				break ;
			}
		}
		std::string nickname = "";
		for (; i < param.length(); i++)
		{
			if (isspace(param[i]))
			{
				i++;
				if (user_ptr == &param[i])
					break ;
			}
			nickname += param[i];
		}
		client.setNickName(nickname);
		if (&param[i] == user_ptr)
		{
			i += 5;	//skip whitespace after USER
			std::string username = "";
			while (i < param.length())
			{
				if (isspace(param[i]))
					break;
				username += param[i];
				i++;
			}
			client.setUserName(username);
			client.setRealName(cmd.message);
			client.setRegistered();
			send(client.getSocket(), "Registration complete. Now you just need the password.\r\n", 57, 0);
			return true;
		}
	}
	return false;
}