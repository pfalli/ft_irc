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

int		removeHash(std::string &tmpChannelTojoin)
{
	size_t	start;

	start = tmpChannelTojoin.find('#');
	if (start == std::string::npos)
		return (-1);
	tmpChannelTojoin = tmpChannelTojoin.substr(start + 1);
	return (0);
}

// static bool	name_check(std::string str)
// {
// 	if (str[0] != '#')
// 		return false;
// 	return true;
// }

void	removeNewline(std::string &str)
{
	size_t pos;
	
	pos = str.find('\n');
	if (pos != std::string::npos)
		str.erase(pos, 1);
}

int		caseK(Server *server, Client *joiningClient, Channel *channel)
{
	char buffer[BUFFER_SIZE];
	std::string key = "Enter key for this channel\r\n";
	send(joiningClient->getSocket(), key.c_str(), key.length(), 0);
	int	bytesReceived = recv(joiningClient->getSocket(), buffer, BUFFER_SIZE, 0);
	if (bytesReceived < 0)
	{
		return (-1);
	}
	std::string input(buffer, bytesReceived);
	removeNewline(input);
	if (input != channel->getKey())
	{
		std::string badKey = ERR_BADCHANNELKEY(server->getName(), joiningClient->getNickName(), channel->getName());
		send(joiningClient->getSocket(), badKey.c_str(), badKey.length(), 0);
		return (-1);
	}
	return (0);
}

void	join(Server *server, Client *joiningClient, std::string channelTojoin)
{
	std::string				username;
	Channel					*channel;

	server->debugging_whoinserver();
	username = joiningClient->getNickName();
	if (channelTojoin.empty())
	{
		sendMsg(joiningClient, ERR_NEEDMOREPARAMS(username));
		return ;
	}
	if (channelTojoin[0] != '#')
	{
		sendMsg(joiningClient, ERR_NOSUCHCHANNEL(server->getName(), joiningClient->getNickName(), channelTojoin));
		return ;
	}
	channel = server->isChannelExist2(channelTojoin);
	if (channel == 0)
	{
		server->createChannel(*joiningClient, channelTojoin, joiningClient->getSocket());
		channel = server->isChannelExist2(channelTojoin);
		if (channel == 0)
			return ;
		channel->joinClient(joiningClient);
		// *** print message in case JOIN_FAILURE, because client already exist in channel ***
		// *** try JOIN !!!channel *** it has to create only channels with '#'
		std::string str = JOIN_SUCCESS(joiningClient->getNickName(), channelTojoin, server->getName(), joiningClient->getUserName());
		send(joiningClient->getSocket(), str.c_str(), strlen(str.c_str()), 0);
		sendToChannel(*channel, str.c_str(), joiningClient->getSocket());
		sendMsg(joiningClient,JOIN_SUCCESS(joiningClient->getNickName(), channelTojoin, server->getName(), joiningClient->getUserName()));
		channel->giveOper(joiningClient->getNickName());
		if (channel->getisTopic() == 1)
		{
			sendMsg(joiningClient, RPL_TOPIC(username, channelTojoin, channel->getTopic()));
			sendMsg(joiningClient, RPL_TOPICWHOTIME(username, channelTojoin, channel->getwhoTopicSet(), channel->getwhenTopicSet()));
		}
		sendMsg(joiningClient, RPL_NAMREPLY(joiningClient->getNickName(), channelTojoin, *channel));
		sendMsg(joiningClient, RPL_ENDOFNAMES(joiningClient->getNickName(), channelTojoin));
		//channel->printAllMembers();
	}
	else
	{
		//channel->printAllMembers();
		if (channel->flagCheck('i') == 0)
		{
			sendMsg(joiningClient,ERR_INVITEONLYCHAN(server->getName(), username, channel->getName()));
			return ;
		}
		if (channel->flagCheck('k') == 0)
		{
			if (caseK(server, joiningClient, channel) == -1)
				return ;
		}
		if (channel->isUserInChannel(joiningClient->getNickName()) || channel->joinClient(joiningClient) == -1)
		{
			sendMsg(joiningClient,ERR_USERONCHANNEL2(joiningClient->getNickName(), joiningClient->getNickName(), channel->getName()));
			return ;
		}
		// std::string welcomemsg_str = JOIN_SUCCESS(joiningClient->getNickName(), channelTojoin, server->getName(), joiningClient->getUserName());
		// const char *welcomemsg = welcomemsg_str.c_str();
		std::string str = JOIN_SUCCESS(joiningClient->getNickName(), channelTojoin, server->getName(), joiningClient->getUserName());
		sendMsg(joiningClient, str.c_str());
		sendToChannel(*channel, str.c_str(), joiningClient->getSocket());
		if (channel->getisTopic() == 1)
		{

			sendMsg(joiningClient,RPL_TOPIC(username, channelTojoin, channel->getTopic()));
			sendMsg(joiningClient,RPL_TOPICWHOTIME(username, channelTojoin, channel->getwhoTopicSet(), channel->getwhenTopicSet()));
		}
		sendMsg(joiningClient,RPL_NAMREPLY(joiningClient->getNickName(), channelTojoin, *channel));
		sendMsg(joiningClient,RPL_ENDOFNAMES(joiningClient->getNickName(), channelTojoin));
	}
}
