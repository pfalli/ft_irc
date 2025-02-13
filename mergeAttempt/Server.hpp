#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>

#include <poll.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>

#include <cerrno>
#include <cstdio>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

class Client
{
	private:
	int						clientSocket;
	std::string				name;

	public:

	void					setName(std::string _name){this->name = _name;}
	std::string				getName() const{return (this->name);}
	int						getSocket()const{return (this->clientSocket);}
};

class Channel{};

class Server
{
private:
	const std::string		name;
	const std::string		password;
	const int				port;
	std::vector<Client>		clients;
	std::vector<Channel>	channels;
	int						serverSocket;
	sockaddr_in				serverAddress;
public:
							Server();
							Server(std::string _password, int _port, std::string _name);
							Server(const Server &src);
							~Server();

	Server 					&operator=(const Server &src);

	int						getPort()const;
	std::string				getName()const;
	std::string				getPassword()const;
	std::vector<Client>		getClients()const;
	std::vector<Channel>	getChannels()const;

	void					startServer();
    void                    launch();
	bool					existingUser(int clientSocket);
	void					initUser(int clientSocket);
	class SeverExceptionSocket : public std::exception
	{
		public:
			const char* what() const throw();
	};
	class SeverExceptionBind : public std::exception
	{
		public:
			const char* what() const throw();
	};
	class SeverExceptionListen : public std::exception
	{
		public:
			const char* what() const throw();
	};
};


#endif