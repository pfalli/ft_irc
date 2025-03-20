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

void sendTopicMsg(Client *client, Channel *channel)
{
	int	fd = client->getSocket();
	if (channel->getisTopic() == 1)
	{
		std::string rtlTopic = RPL_TOPIC(client->getNickName(), channel->getName(), channel->getTopic());
		send(fd, rtlTopic.c_str(), rtlTopic.length(), 0);
		std::string whoTime = RPL_TOPICWHOTIME(channel->getName(), client->getNickName(), channel->getwhoTopicSet(), channel->getwhenTopicSet());
		send(fd, whoTime.c_str(), whoTime.length(), 0);
		return ;
	}
	else
	{
		std::string rplNotopic = RPL_NOTOPIC(client->getNickName(), channel->getName());
		send(fd, rplNotopic.c_str(), rplNotopic.length(), 0);
		return ;
	}
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
		sendTopicMsg(client, channel);
		return ;
	}
	else
	{
		if (channel->flagCheck('t') == 0 && isOperator(*channel, client) == false)
		{
			sendMsg(client, ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getName()));
			return ;
		}
		if (cmd.message.size() == 1 && cmd.message[0] == ':')
		{
			channel->clearTopic(channel, client);
			return ;
		}
		else
			channel->setTopic(cmd.message, nickName);
		sendToChannel(*channel, TOPIC_SET(client->getNickName(), client->getUserName(),  channelName, channel->getTopic()), client->getSocket());
		sendMsg(client, TOPIC_SET(client->getNickName(), client->getUserName(),  channelName, channel->getTopic()));
		return ;
	}
}