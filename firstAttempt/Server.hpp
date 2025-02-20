/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:07:47 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/13 13:57:35 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <istream>

class Client
{
	private:
	int						clientSocket;
	std::string				name;

	public:

	void					setName(std::string _name){this->name = _name;}
	std::string				getName() const{return (this->name);}
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
	void					initUser(int clientSocket);
};


