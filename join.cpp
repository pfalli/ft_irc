/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:35:49 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/28 16:08:51 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"
#include "Channel.hpp"

/* Check if channelTojoin exist in the Server 
if does not exist, make a new one and return its reference 
if does exist, return its reference */

// Remove #
int		removeHash(std::string &tmpChannelTojoin)
{
	size_t	start;

	start = tmpChannelTojoin.find('#');
	if (start == std::string::npos)
		return (-1);
	tmpChannelTojoin = tmpChannelTojoin.substr(start + 1);
	return (0);
}

Channel*		isChannelExist(std::vector<Channel>&	channels, std::string tmpChannelTojoin)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->getName() == tmpChannelTojoin) // if channel exists
			return &(*it);
	}
	return (0);
}

void	join(Server *server, Client *joiningClient, std::string channelTojoin)
{
	std::vector<Channel>&	channels = server->getChannelsref();
	std::string				username;
	std::string				tmpChannelTojoin;
	Channel					*channel;

	username = joiningClient->getUserName();
	tmpChannelTojoin = channelTojoin;
	if (removeHash(tmpChannelTojoin) != 0)
	{
		std::string	hashMsg = ERR_NEEDMOREPARAMS2(username, "JOIN");
		send(joiningClient->getSocket(), hashMsg.c_str(), hashMsg.length(), 0);
		return ;
	}
	channel = isChannelExist(channels, tmpChannelTojoin);
	if (channel == 0)
	{
		server->createChannel(tmpChannelTojoin, joiningClient->getSocket());
		channel = isChannelExist(channels, tmpChannelTojoin);
		channel->joinClient(*joiningClient);
		std::string welcomemsg = JOIN_SUCCESS(joiningClient->getNickName(), tmpChannelTojoin);
		sendToChannel(*channel, welcomemsg);
		if (channel->getisTopic() == 1)
		{
			std::string topicMsg = RPL_TOPIC(username, tmpChannelTojoin, channel->getTopic());
			std::string topicMsg2 = RPL_TOPICWHOTIME(username, tmpChannelTojoin, channel->getwhoTopicSet(), channel->getwhenTopicSet());
			send(joiningClient->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
			send(joiningClient->getSocket(), topicMsg2.c_str(), topicMsg2.length(), 0);
		}
		std::string	memberInfo = RPL_NAMREPLY(joiningClient->getNickName(), tmpChannelTojoin, *channel);
		send(joiningClient->getSocket(), memberInfo.c_str(), memberInfo.length(), 0);
	}
	else
	{
		channel->joinClient(*joiningClient);
		std::string welcomemsg = JOIN_SUCCESS(joiningClient->getNickName(), tmpChannelTojoin);
		sendToChannel(*channel, welcomemsg);
		if (channel->getisTopic() == 1)
		{
			std::string topicMsg = RPL_TOPIC(username, tmpChannelTojoin, channel->getTopic());
			std::string topicMsg2 = RPL_TOPICWHOTIME(username, tmpChannelTojoin, channel->getwhoTopicSet(), channel->getwhenTopicSet());
			send(joiningClient->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
			send(joiningClient->getSocket(), topicMsg2.c_str(), topicMsg2.length(), 0);
		}
		std::string	memberInfo = RPL_NAMREPLY(joiningClient->getNickName(), tmpChannelTojoin, *channel);
		send(joiningClient->getSocket(), memberInfo.c_str(), memberInfo.length(), 0);
	}
}
