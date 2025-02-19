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
		if (!isalnum(str[i]))
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
		if (!isalnum(str[i]))
			return (false);
	}
	return (true);
}
static bool validPassword(std::string str)
{
	if (strlen(str.c_str()) > 20 || strlen(str.c_str()) < 8)
		return (false);
	for (size_t i = 0; i < strlen(str.c_str()); i++)
	{
		if (!isalnum(str[i]) || (str[i] <= 40 && str[i] >= 21) )
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
std::string					removeNewline(char *buff)
{
	std::string _new = "";

	for (size_t i = 0; i < strlen(buff); i++)
	{
		if (!isalnum(buff[i]) && buff[i] != ' ')
			break;
		_new += buff[i];
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