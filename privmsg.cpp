/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:12:42 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/26 14:59:02 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Common.hpp"

void	messageToTargets(Client *sender, Server *server, std::vector<std::string> &targets, std::string msg)
{
	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
	{
		for (std::vector<Client>::iterator it2 = server->getClients().begin(); it2 != server->getClients().end(); it2++)
		{
			std::cout << "target : " << *it << std::endl;
			if (*it == it2->getNickName())
			{
				send(it2->getSocket(), msg.c_str(), msg.length(), 0);
				break ;
			}
			if (it2 == server->getClients().end())
			{
				std::cout << "does not exist" << std::endl;
				std::string errmsg = std::string(RED) + *it + " Does not exist\n" RESET;
				send(sender->getSocket(), errmsg.c_str(), errmsg.length(), 0);
			}
		}
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

/* find colon */
// int	parseTarget(Client *sender, std::string &tmpTarget)
// {
// 	size_t	locationColon;
// 	locationColon = tmpTarget.find(':');
// 	if (locationColon == std::string::npos)
// 	{
// 		std::string msg =  RED "Colon missing. correct format : <target> : <message>\n" RESET;
// 		send(sender->getSocket(), msg.c_str(), msg.length(), 0);
// 		return (1);
// 	}
// 	tmpTarget = tmpTarget.substr(0, locationColon);
// 	return (0);
// }

// void	parseMessage(std::string &message)
// {
// 	size_t	locationColon;

// 	locationColon = message.find(':');
// 	message = message.substr(locationColon, message.size());
// }

void	privmsg(Server *server, Client *sender, const Command &cmd)
{
	std::string message;
	std::vector<std::string> targets;

	splitTargetUsers(cmd.parameter, targets);
	messageToTargets(sender, server, targets, cmd.message);
	for (size_t i = 0; i < targets.size(); ++i) {
		std::cout << "User " << i << ": " << targets[i] << std::endl;
	}
}