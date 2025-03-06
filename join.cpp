/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:35:49 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/28 16:54:01 by junhhong         ###   ########.fr       */
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

Channel*		isChannelExist(std::vector<Channel>&	channels, std::string channelTojoin)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::string tmp = it->getName();
		std::cout << "tmp : "  << tmp<< std::endl;
		std::cout << "toJoin : "  << channelTojoin << std::endl;
		if (tmp == channelTojoin) // if channel exists
			return &(*it);
	}
	return (0);
}

static bool	name_check(std::string str)
{
	if (str[0] != '#')
		return false;
	return true;
}

void	join(Server *server, Client *joiningClient, std::string channelTojoin)
{
	std::vector<Channel>&	channels = server->getChannelsref();
	std::string				username;
	//std::string				tmpChannelTojoin;
	Channel					*channel;

	username = joiningClient->getNickName();
	//tmpChannelTojoin = channelTojoin;
	// if (removeHash(tmpChannelTojoin) != 0)
	// {
	// 	std::string	hashMsg = ERR_NEEDMOREPARAMS2(username, "JOIN");
	// 	send(joiningClient->getSocket(), hashMsg.c_str(), hashMsg.length(), 0);
	// 	return ;
	// }

	if (!name_check(channelTojoin))
			channelTojoin = "#" + channelTojoin;	//automatic # added to avoid crash of KICK

	channel = isChannelExist(channels, channelTojoin);
	if (channel == 0)
	{
		server->createChannel(*joiningClient, channelTojoin, joiningClient->getSocket());
		channel = isChannelExist(channels, channelTojoin);
		channel->joinClient(*joiningClient);
		// *** print message in case JOIN_FAILURE, because client already exist in channel ***
		// *** try JOIN !!!channel *** it has to create only channels with '#'
		std::string welcomemsg = JOIN_SUCCESS(joiningClient->getNickName(), channelTojoin);
		sendToChannel(*channel, welcomemsg);
		if (channel->getisTopic() == 1)
		{
			std::string topicMsg = RPL_TOPIC(username, channelTojoin, channel->getTopic());
			std::string topicMsg2 = RPL_TOPICWHOTIME(username, channelTojoin, channel->getwhoTopicSet(), channel->getwhenTopicSet());
			send(joiningClient->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
			send(joiningClient->getSocket(), topicMsg2.c_str(), topicMsg2.length(), 0);
		}
		std::string	memberInfo = RPL_NAMREPLY(joiningClient->getNickName(), channelTojoin, *channel);
		std::string endOfMsg = RPL_ENDOFNAMES(joiningClient->getNickName(), channelTojoin);
		send(joiningClient->getSocket(), memberInfo.c_str(), memberInfo.length(), 0);
		send(joiningClient->getSocket(), endOfMsg.c_str(), endOfMsg.length(), 0);
	}
	else
	{
		channel->joinClient(*joiningClient);

		std::string welcomemsg = JOIN_SUCCESS(joiningClient->getNickName(), channelTojoin);
		sendToChannel(*channel, welcomemsg);
		if (channel->getisTopic() == 1)
		{
			std::string topicMsg = RPL_TOPIC(username, channelTojoin, channel->getTopic());
			std::string topicMsg2 = RPL_TOPICWHOTIME(username, channelTojoin, channel->getwhoTopicSet(), channel->getwhenTopicSet());
			send(joiningClient->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
			send(joiningClient->getSocket(), topicMsg2.c_str(), topicMsg2.length(), 0);
		}
		std::string	memberInfo = RPL_NAMREPLY(joiningClient->getNickName(), channelTojoin, *channel);
		std::string endOfMsg = RPL_ENDOFNAMES(joiningClient->getNickName(), channelTojoin);
		send(joiningClient->getSocket(), memberInfo.c_str(), memberInfo.length(), 0);
		send(joiningClient->getSocket(), endOfMsg.c_str(), endOfMsg.length(), 0);
	}
}
