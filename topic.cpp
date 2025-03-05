/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:42:02 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/28 15:57:40 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "utils.hpp"

void sendTopicMsg(Client &client, Channel *channel, std::string userName)
{
	int	fd = client.getSocket();
	if (channel->getisTopic() == 1)
	{
		std::string rtlTopic = RPL_TOPIC(userName, channel->getName(), channel->getTopic());
		send(fd, rtlTopic.c_str(), rtlTopic.length(), 0);
		std::string whoTime = RPL_TOPICWHOTIME(userName, channel->getName(), channel->getwhoTopicSet(), channel->getwhenTopicSet());
		send(fd, whoTime.c_str(), whoTime.length(), 0);
		return ;
	}
	else
	{
		std::string rplNotopic = RPL_NOTOPIC(userName, channel->getName());
		send(fd, rplNotopic.c_str(), rplNotopic.length(), 0);
		return ;
	}
}

int	isUserInTheChannel(std::string userName, Channel *channel)
{
	std::vector<Client *> &clients = channel->getJoinedClients();

	std::vector<Client *>::iterator it = clients.begin();
	for (; it != clients.end(); it++)
	{
		if ((*it)->getUserName() == userName)
			return (0);
	}
	return (-1);
}

void topic(Server *server, const Command &cmd, Client &client)
{
	std::string	channelName = cmd.parameter;
	std::string	userName = client.getUserName();
	Channel *channel;

	channel = isChannelExist(server->getChannelsref(), channelName);
	if (channel == 0) // if channel does not exist
	{
		std::string noSuchChannel = ERR_NOSUCHCHANNEL(userName, channelName);
		send (client.getSocket(), noSuchChannel.c_str(), noSuchChannel.length(), 0);
		return ;
	}
	if (isUserInTheChannel(userName, channel) == -1) // check if user in the channel
	{
		std::string notToChannel = ERR_NOTONCHANNEL(userName, channelName);
		send (client.getSocket(), notToChannel.c_str(), notToChannel.length(), 0);
		return ;
	}
	if (cmd.hasMessage == 0)
	{
		sendTopicMsg(client, channel, userName);
		return ;
	}
	else 
	{
		if (cmd.message.empty())
			channel->clearTopic(userName);
		else
			channel->setTopic(cmd.message, userName);
		sendToChannel(*channel, RPL_TOPIC(userName, channelName, channel->getTopic()));
		sendToChannel(*channel, RPL_TOPICWHOTIME(userName, channelName, channel->getwhoTopicSet(), channel->getwhenTopicSet()));
		return ;
	}
} 