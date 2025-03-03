/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:47:21 by junhhong          #+#    #+#             */
/*   Updated: 2025/03/03 17:47:18 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "utils.hpp"

int		modeParse(const Command &cmd, modeCommand& parsedModeCommand)
{
	std::istringstream	iss;
	std::string			parameter;

	parameter = cmd.parameter;
	iss.str(parameter);

	iss >> parsedModeCommand.channelName;
	iss >> parsedModeCommand.mode;
	std::getline(iss >> std::ws, parsedModeCommand.modeArgument);
	if (parsedModeCommand.mode[0] == '+')
		parsedModeCommand.setRemove = 1;
	else if (parsedModeCommand.mode[0] == '-')
		parsedModeCommand.setRemove = -1;
	
	std::cout << "!------------------------------------------------------------!" << std::endl;
	std::cout << "channelName :" << parsedModeCommand.channelName << std::endl;
	std::cout << "mode :" << parsedModeCommand.mode << std::endl;
	std::cout << "modeArgument :" << parsedModeCommand.modeArgument << std::endl;
	std::cout << "setRemove :" << parsedModeCommand.setRemove << std::endl;
	std::cout << "!------------------------------------------------------------!" << std::endl;
	return (0);
}

void	printModes(Server *server, modeCommand &parsedModeCommand, Client &client, Channel &channel)
{
	std::string channelModeIs = RPL_CHANNELMODEIS(server->getName(), client.getNickName(), channel.getName(), channel.getModes(), parsedModeCommand.modeArgument);
	std::string creationTime = RPL_CREATIONTIME(server->getName(), client.getNickName(), channel.getName(), channel.getserverCreationTime());
	send(client.getSocket(), channelModeIs.c_str(), channelModeIs.length(), 0);
	send(client.getSocket(), creationTime.c_str(), creationTime.length(), 0);
}

void	mode(Server *server, const Command &cmd, Client &client)
{
	Channel		*channel;
	modeCommand	parsedModeCommand;
	std::string	channelName = cmd.parameter;
	std::string	userName = client.getUserName();
	
	modeParse(cmd, parsedModeCommand);
	channel = isChannelExist(server->getChannelsref(), parsedModeCommand.channelName);
	if (channel == 0) // if channel does not exist
	{
		std::string noSuchChannel = ERR_NOSUCHCHANNEL(userName, channelName);
		send (client.getSocket(), noSuchChannel.c_str(), noSuchChannel.length(), 0);
		return ;
	}
	if (parsedModeCommand.mode.empty())
	{
		printModes(server, parsedModeCommand, client, *channel);
		return ;
	}
}