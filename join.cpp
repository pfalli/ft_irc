/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:35:49 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/24 17:08:47 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"

/* Check if channelTojoin exist in the Server 
if does not exist, make a new one and return its reference 
if does exist, return its reference */
void	join(Server *server, Client *joiningClient, std::string channelTojoin)
{
	std::vector<Channel>&	channels = server->getChannelsref();
	std::string				username;

	username = joiningClient->getUserName();
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getName() == channelTojoin) // if channel exists
		{
			it->joinClient(*joiningClient);
			return ;
		}
		if (it == channels.end())
		{
			server->createChannel(channelTojoin);
			it->joinClient(*joiningClient);
			return ;
		}
	}
}
