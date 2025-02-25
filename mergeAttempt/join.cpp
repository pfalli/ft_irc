/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:35:49 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/25 15:48:41 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"

/* Check if channelTojoin exist in the Server 
if does not exist, make a new one and return its reference 
if does exist, return its reference */

// Remove #
int	parseChannelName(std::string tmpChannelTojoin)
{
	size_t	start;

	start = tmpChannelTojoin.find('#');
	if (start == std::string::npos)
		return (-1);
	tmpChannelTojoin.substr(start);
	return (0);
}

int		isChannleExist(std::vector<Channel>&	channels, Client *joiningClient, std::string tmpChannelTojoin)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->getName() == tmpChannelTojoin) // if channel exists
		{
			it->joinClient(*joiningClient);
			std::cout << "you are joined to" << tmpChannelTojoin << std::endl; 
			return (0);
		}
	}
	return (1);
}

void	join(Server *server, Client *joiningClient, std::string channelTojoin)
{
	std::vector<Channel>&	channels = server->getChannelsref();
	std::string				username;
	std::string				tmpChannelTojoin;

	tmpChannelTojoin = channelTojoin;
	if (parseChannelName(tmpChannelTojoin) != 0)
	{
		std::cout << "Channel name should start with '#'" << std::endl; 
		return ;
	}
	username = joiningClient->getUserName();
	if (isChannleExist(channels, joiningClient, tmpChannelTojoin) != 0)
	{
		server->createChannel(tmpChannelTojoin);
		std::cout << "Channel does not exist." << tmpChannelTojoin << " created" << std::endl; 
	}
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->getName() == tmpChannelTojoin) // if channel exists
		{
			it->joinClient(*joiningClient);
			std::string msg = "you joined " + tmpChannelTojoin + " channel. enjoy!\n\n\n";
			send(joiningClient->getSocket(), msg.c_str(), msg.length(), 0);
			std::cout << "you are joined to" << tmpChannelTojoin << std::endl; 
			return ;
		}
	}
}
