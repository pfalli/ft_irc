#ifndef SERVER_HPP
# define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "Common.hpp"

// #include <iostream>
// #include <sys/types.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <arpa/inet.h>
// #include <string.h>
// #include <string>
// #include <vector>

// #include <poll.h>
// #include <netinet/in.h>
// #include <cstdlib>
// #include <cstring>
// #include <sys/types.h>

// #include <cerrno>
// #include <cstdio>
// #include <pthread.h>
// #include <sstream>

// #define BUFFER_SIZE 1024
// #define MAX_CLIENTS 10

// #define FAILURE -1
// #define POLL_TIMEOUT 0
// #define SUCCESS 1
// #define DISCONNECT 2
// #define IGNORE 5

// class Client
// {
// 	private:
// 	int										clientSocket; //same as client_fd
// 	std::string								user_name;
// 	std::string								nick_name;

// 	public:
// 											Client(){user_name = "default"; nick_name = "default_nick";}
// 											~Client(){};
// 	void									setUserName(std::string _name){this->user_name = _name;}
// 	void									setNickName(std::string _name){this->nick_name = _name;}
// 	void									setSocket(int socket){this->clientSocket = socket;}
// 	std::string								getUserName() const{return (this->user_name);}
// 	std::string								getNickName() const{return (this->nick_name);}
// 	int										getSocket()const{return (this->clientSocket);}
// };

// class Channel{
// 	private:
// 	std::string								name;
// 	std::string								nameClientList;
// 	std::vector<Client>						_joinedClients;

// 	public:
// 	void									setName(std::string _name){this->name = _name;}
// 	void									joinClient(const Client &client);
// 	std::string								getName()const{return (this->name);}
// 	std::vector<Client>&					getJoinedClients(){return (this-> _joinedClients);}
// };

struct Command {
    std::string command;
    std::string parameter;
    std::string message;
};

class Server
{
	private:
	bool									server_shutdown;
	const std::string						name;
	const std::string						password;
	const int								port;
	std::vector<Client>						clients;
	std::vector<Channel>					channels; // **#team
	int										serverSocket;
	sockaddr_in								serverAddress;
	std::vector<struct pollfd>				poll_fds;

	public:
											Server();
											Server(std::string _password, int _port, std::string _name);
											Server(const Server &src);
											~Server();

	Server 									&operator=(const Server &src);

	int										getPort()const;
	std::string								getName()const;
	std::string								getPassword()const;
	std::vector<Client>						getClients()const;
	std::vector<Channel>					getChannels()const;
	std::vector<Channel>&					getChannelsref();
	bool									getServerShutdown()const;

	void									createChannel(std::string name);

	bool 									registration(std::vector<Client>::iterator it);
	void									startServer();
	void									launch();
	bool									existingUser(int clientSocket);
	std::string								requestName(int format, int clientSocket);
	void									getMessages();
	int										NewClient(int new_socket);
	int										clientDisconnect(int client_fd, std::vector<pollfd>::iterator it, int bytes_read);
	int										acceptClient();
	bool									sendToNext(std::string buff, int client_fd);
	void									existingConnection(std::vector<pollfd>::iterator it);
	void									newConnection();

	void parseCommand(const std::string &str, Command &cmd);
	void handleCommand(const Command &cmd);



	void									deleteClient(std::vector<Client>::iterator client, std::vector<pollfd>::iterator poll);
	template <typename T>
	typename std::vector<T>::iterator		findObject(int toFind, std::vector<T> &array);
};


class SeverExceptionSocket : public std::exception
{
	public:
		const char*						what() const throw();
};
class SeverExceptionBind : public std::exception
{
	public:
		const char*						what() const throw();
};
class SeverExceptionListen : public std::exception
{
	public:
		const char*						what() const throw();
};
class FailedPollException : public std::exception
{
	public:
		const char*						what() const throw();
};

#endif