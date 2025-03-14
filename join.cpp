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
		std::string str = ERR_NEEDMOREPARAMS(joiningClient->getUserName());
		const char *msg = str.c_str();
		send(joiningClient->getSocket(), msg, strlen(msg), 0);
		return ;
	}
	if (channelTojoin[0] != '#')
	{
		send(joiningClient->getSocket(), "Error: channel name needs to include # at beginning\n", 53, 0);
		return ;
	}
	channel = server->isChannelExist2(channelTojoin);
	if (channel != 0)
		channel->printAllMembers();
	if (channel == 0)
	{
		server->createChannel(*joiningClient, channelTojoin, joiningClient->getSocket());
		channel = server->isChannelExist2(channelTojoin);
		channel->joinClient(joiningClient);
		// *** print message in case JOIN_FAILURE, because client already exist in channel ***
		// *** try JOIN !!!channel *** it has to create only channels with '#'
		std::string welcomemsg_str = JOIN_SUCCESS(joiningClient->getNickName(), channelTojoin, server->getName(), joiningClient->getUserName());
		const char *welcomemsg = welcomemsg_str.c_str();
		sendToChannel(*channel, welcomemsg);
		
		if (channel->getisTopic() == 1)
		{
			std::string topicMsg_str = RPL_TOPIC(username, channelTojoin, channel->getTopic());
			const char *topicMsg = topicMsg_str.c_str();
			std::string topicMsg2_str = RPL_TOPICWHOTIME(username, channelTojoin, channel->getwhoTopicSet(), channel->getwhenTopicSet());
			const char *topicMsg2 = topicMsg2_str.c_str();
			send(joiningClient->getSocket(), topicMsg, strlen(topicMsg), 0);
			send(joiningClient->getSocket(), topicMsg2, strlen(topicMsg2), 0);
		}
		std::string	memberInfo_str = RPL_NAMREPLY(joiningClient->getNickName(), channelTojoin, *channel);
		const char *memberInfo = memberInfo_str.c_str();
		std::string endOfMsg_str = RPL_ENDOFNAMES(joiningClient->getNickName(), channelTojoin);
		const char *endOfMsg = endOfMsg_str.c_str();
		send(joiningClient->getSocket(), memberInfo, strlen(memberInfo), 0);
		send(joiningClient->getSocket(), endOfMsg, strlen(endOfMsg), 0);
		channel->printAllMembers();
	}
	else
	{
		channel->printAllMembers();
		if (channel->flagCheck('i') == 0)
		{
			std::string joinOnly_str = ERR_INVITEONLYCHAN(server->getName(), username, channel->getName());
			const char *joinOnly = joinOnly_str.c_str();
			send(joiningClient->getSocket(), joinOnly, strlen(joinOnly), 0);
			return ;
		}
		if (channel->flagCheck('k') == 0)
		{
			if (caseK(server, joiningClient, channel) == -1)
			{
				return ;
			}
		}
		if (channel->isUserInChannel(joiningClient->getNickName()))
		{
			std::string usrOnChannel = ERR_USERONCHANNEL(server->getName(), joiningClient->getNickName(), channel->getName());
			send(joiningClient->getSocket(), usrOnChannel.c_str(), usrOnChannel.length(), 0);
			return ;
		}
		if (channel->joinClient(joiningClient) == -1)
		{
			std::string usrOnChannel2 = ERR_USERONCHANNEL(server->getName(), joiningClient->getNickName(), channel->getName());
			send(joiningClient->getSocket(), usrOnChannel2.c_str(), usrOnChannel2.length(), 0);
			return ;
		}
		std::string welcomemsg_str = JOIN_SUCCESS(joiningClient->getNickName(), channelTojoin, server->getName(), joiningClient->getUserName());
		const char *welcomemsg = welcomemsg_str.c_str();
		sendToChannel(*channel, welcomemsg);
		if (channel->getisTopic() == 1)
		{
			std::string topicMsg_str = RPL_TOPIC(username, channelTojoin, channel->getTopic());
			const char *topicMsg = topicMsg_str.c_str();
			std::string topicMsg2_str = RPL_TOPICWHOTIME(username, channelTojoin, channel->getwhoTopicSet(), channel->getwhenTopicSet());
			const char *topicMsg2 = topicMsg2_str.c_str();
			send(joiningClient->getSocket(), topicMsg, strlen(topicMsg), 0);
			send(joiningClient->getSocket(), topicMsg2, strlen(topicMsg2), 0);
		}
		std::string	memberInfo_str = RPL_NAMREPLY(joiningClient->getNickName(), channelTojoin, *channel);
		const char *memberInfo = memberInfo_str.c_str();
		std::string endOfMsg_str = RPL_ENDOFNAMES(joiningClient->getNickName(), channelTojoin);
		const char *endOfMsg = endOfMsg_str.c_str();
		send(joiningClient->getSocket(), memberInfo, strlen(memberInfo), 0);
		send(joiningClient->getSocket(), endOfMsg, strlen(endOfMsg), 0);
	}
}
