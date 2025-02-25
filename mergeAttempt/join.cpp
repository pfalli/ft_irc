/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:35:49 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/25 15:04:06 by junhhong         ###   ########.fr       */
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

void	join(Server *server, Client *joiningClient, std::string channelTojoin)
{
	std::vector<Channel>&	channels = server->getChannelsref();
	std::string				username;
	std::string				tmpChannelTojoin;

	tmpChannelTojoin = channelTojoin;
	std::cout << "join in" << std::endl; 
	std::cout << "channelTojoin: " << channelTojoin << std::endl; 
	std::cout << "tmpChannelTojoin: " << tmpChannelTojoin << std::endl; 
	if (parseChannelName(tmpChannelTojoin) != 0)
	{
		std::cout << "Channel name should start with '#'" << std::endl; 
		return ;
	}
	username = joiningClient->getUserName();
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->getName() == tmpChannelTojoin) // if channel exists
		{
			it->joinClient(*joiningClient);
			std::cout << "you are joined to" << tmpChannelTojoin << std::endl; 
			return ;
		}
	}
	server->createChannel(tmpChannelTojoin);
	std::cout << "making new channel : " << tmpChannelTojoin << std::endl; 
	it->joinClient(*joiningClient);
	std::cout << "you are joined to" << tmpChannelTojoin << std::endl; 
	return ;

}
