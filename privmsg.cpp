/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:12:42 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/28 14:29:42 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Common.hpp"

#include "utils.hpp"
#include "Common.hpp"
#include "NumericMessages.hpp"


/* find if nickName is in server. 
	if exist, return socket
	if not, return -1 */
int isUserInServer(Server *server, std::string nickName)
{
	std::vector<Client> clients = server->getClients();
	
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getNickName() == nickName)
			return it->getSocket();
	}
	return -1;
}

void	messageToTargets(Client *sender, Server *server, std::vector<std::string> &targets, std::string msg)
{
	for (std::vector<std::string>::iterator recipient = targets.begin(); recipient != targets.end(); recipient++)
	{
		int	fd = isUserInServer(server, *recipient);
		if (fd != -1) // if exist
		{
			std::string success_message = SUCCESS_PRIVMSG(sender->getNickName(), *recipient, msg);
			send(fd, success_message.c_str(), success_message.length(), 0);
			send(sender->getSocket(), success_message.c_str(), success_message.length(), 0);
		}
		else
			send(sender->getSocket(), ERR_NOSUCHNICK(sender->getNickName(), *recipient).c_str(), ERR_NOSUCHNICK(sender->getNickName(), *recipient).length(), 0);
	}
}

void	splitTargetUsers(std::string tmpTarget, std::vector<std::string> &targets)
{
	std::istringstream	iss(tmpTarget);
	std::string			user;

	while (iss >> user)
	{
		targets.push_back(user);
	}
}

// void	parseChannelName2(std::string &targetChannel)
// {
// 	size_t	location;

// 	location = targetChannel.find('#');
// 	targetChannel = targetChannel.substr(location + 1, targetChannel.length());

// 	std::cout << "targetChannel :" << targetChannel << std::endl;
// }

void	sendToChannel(Channel &channel, const char *msg)
{
	std::vector<Client *> clients = channel.getJoinedClients();
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end() ;it++)
	{
		send((*it)->getSocket(), msg, strlen(msg), 0);
	}
}


// void	sendToChannel(Channel &channel, std::string senderNickname, std::string channelName, std::string msg)
// {
// 	std::vector<Client> clients = channel.getJoinedClients();
// 	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end() ;it++)
// 	{
// 		std::string	msgToChannel = TO_ALL_CHANNEL(senderNickname, channelName, msg);
// 		send(it->getSocket(), msgToChannel.c_str(), msgToChannel.length(), 0);
// 	}
// }

void	messageToAllChannel(Server *server, Client *sender, const Command &cmd)
{
	std::vector<Channel> channels = server->getChannels();
	std::string	targetChannel = cmd.parameter;
	std::string	senderNickname = sender->getNickName();
	std::string senderUser = sender->getUserName();

	//parseChannelName2(targetChannel);
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->getName() == targetChannel)
		{
			//std::string msg = TO_ALL_CHANNEL(senderNickname, targetChannel, cmd.message);
			std::string msg = TO_ALL_CHANNEL_TEST(senderNickname, targetChannel, cmd.message, senderUser, server->getName());
			const char *message = msg.c_str();
			sendToChannel(*it, message);
			return ;
		}
	}
	std::string errmsg = ERR_NOSUCHCHANNEL(server->getName(), sender->getUserName(), targetChannel);
	send(sender->getSocket(), errmsg.c_str(), errmsg.length(), 0);
}

void	privmsg(Server *server, Client *sender, const Command &cmd)
{
	std::string message;
	std::vector<std::string> targets;

	splitTargetUsers(cmd.parameter, targets);
	if (cmd.parameter[0] == '#')
	{
		Channel *channel;
		channel = server->isChannelExist2(cmd.parameter);
		if (channel == NULL)
		{
			std::string errmsg = ERR_NOSUCHCHANNEL(server->getName(), sender->getUserName(), cmd.parameter);
			send(sender->getSocket(), errmsg.c_str(), errmsg.length(), 0);
			return ;
		}
		else
		{
			if (channel->isUserInChannel(sender->getNickName()) == NULL)
			{
				std::string notOnChannel = ERR_NOTONCHANNEL(server->getName(), channel->getName());
				send(sender->getSocket(), notOnChannel.c_str(), notOnChannel.length(), 0);
				return ;
			}
		}
		messageToAllChannel(server, sender, cmd);
		return ;
	}
	messageToTargets(sender, server, targets, cmd.message);
}