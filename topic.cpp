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

void sendTopicMsg(Client *client, Channel *channel, std::string userName)
{
	int	fd = client->getSocket();
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

int	topicProtected(Server *server, Channel *channel, Client *client)
{
	(void) server; // fix it later
	std::string errmsg = ERR_CHANOPRIVSNEEDED(server->getName(), client->getNickName(), channel->getName());
	if (channel->flagCheck('t') == 0)
	{
		if (channel->hasOper(client) != NULL)
			return (1);
	}
	send(client->getSocket(), errmsg.c_str(), errmsg.length(), 0);
	return (-1);
} 

void topic(Server *server, const Command &cmd, Client *client)
{
	std::string	channelName = cmd.parameter;
	std::string	nickName = client->getNickName();
	Channel *channel;

	channel = server->isChannelExist2(channelName);
	if (channel == 0) // if channel does not exist
	{
		std::string noSuchChannel = ERR_NOSUCHCHANNEL(nickName, channelName);
		send (client->getSocket(), noSuchChannel.c_str(), noSuchChannel.length(), 0);
		return ;
	}
	if (channel->isUserInChannel(nickName) == NULL) // check if user in the channel
	{
		std::string notToChannel = ERR_NOTONCHANNEL(nickName, channelName);
		send (client->getSocket(), notToChannel.c_str(), notToChannel.length(), 0);
		return ;
	}
	if (cmd.hasMessage == 0)
	{
		sendTopicMsg(client, channel, nickName);
		return ;
	}
	else
	{
		if (topicProtected(server, channel, client) == -1)
			return ;
		if (cmd.message.empty())
			channel->clearTopic(nickName);
		else
			channel->setTopic(cmd.message, nickName);
		std::string str = RPL_TOPIC(nickName, channelName, channel->getTopic());
		const char *msg1 = str.c_str();
		sendToChannel(*channel, msg1, client->getSocket());
		str = RPL_TOPICWHOTIME(nickName, channelName, channel->getwhoTopicSet(), channel->getwhenTopicSet());
		const char *msg2 = str.c_str();
		sendToChannel(*channel, msg2, client->getSocket());
		return ;
	}
}