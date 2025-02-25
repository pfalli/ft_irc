/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:12:42 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/25 18:00:56 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Common.hpp"

// void	messageToTargets(std::vector<std::string> &targets)
// {
	
// }

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
int	parseTarget(Client *sender, std::string &tmpTarget)
{
	size_t	locationColon;
	locationColon = tmpTarget.find(':');
	if (locationColon == std::string::npos)
	{
		std::string msg =  RED "Colon missing. correct format : <target> : <message>\n" RESET;
		send(sender->getSocket(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	tmpTarget = tmpTarget.substr(0, locationColon);
	return (0);
}

void	privmsg(Server *server, Client *sender, std::string cmd)
{
	std::string tmpTarget;
	std::vector<std::string> targets;

	(void) server;
	tmpTarget = cmd;
	if (parseTarget(sender, tmpTarget) != 0)
		return ;
	splitTargetUsers(tmpTarget, targets);
    for (size_t i = 0; i < targets.size(); ++i) {
        std::cout << "User " << i << ": " << targets[i] << std::endl;
    }
}