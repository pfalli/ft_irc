/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:34:13 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/13 13:56:34 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>

int main()
{
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	char buff[1024];
	connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	recv(clientSocket, buff, sizeof(buff), 0);
	{
		std::string message = buff;
		std::cout << message << std::endl;
	}
	std::string input;
	std::cin >> input;
	send(clientSocket, input.c_str(), strlen(input.c_str()), 0);
	close(clientSocket);
}