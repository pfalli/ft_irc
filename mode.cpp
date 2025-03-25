/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:47:21 by junhhong          #+#    #+#             */
/*   Updated: 2025/03/05 18:24:58 by junhhong         ###   ########.fr       */
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
	
	// std::cout << "!------------------------------------------------------------!" << std::endl;
	// std::cout << "channelName :" << parsedModeCommand.channelName << std::endl;
	// std::cout << "mode :" << parsedModeCommand.mode << std::endl;
	// std::cout << "modeArgument :" << parsedModeCommand.modeArgument << std::endl;
	// std::cout << "setRemove :" << parsedModeCommand.setRemove << std::endl;
	// std::cout << "!------------------------------------------------------------!" << std::endl;
	return (0);
}

std::string modeCommandMaker(Channel &channel)
{
	std::string modeString = channel.getModes();
	std::vector<Client *> operators = channel.getOperators();
	size_t	i = 0;
	for(; i < channel.getModes().length(); i++)
	{
		if (modeString[i] == 'l')
			modeString += " " + convertSizeTtoString(channel.getLimit());
		if (modeString[i] == 'k')
			modeString += " " + channel.getKey();
		while (modeString[i] == 'o' && i < channel.getModes().length())
		{
			modeString += " " + operators[i]->getNickName();
			i ++;
		}
	}
	return (modeString);
}

void	printModes(Client *client, Channel &channel, int to_all)
{
	std::string channelModeIs;
	std::string	modeString;
	modeString = modeCommandMaker(channel);
	channelModeIs = RPL_CHANNELMODEIS(serverName, client->getNickName(), channel.getName(), modeString);
	std::string creationTime = RPL_CREATIONTIME(serverName, client->getNickName(), channel.getName(), channel.getserverCreationTime());
	if (to_all == 0)
	{	
		send(client->getSocket(), channelModeIs.c_str(), channelModeIs.length(), 0);
		send(client->getSocket(), creationTime.c_str(), creationTime.length(), 0);
		return ;
	}
	else
	{
		const char *msg1 = channelModeIs.c_str();
		const char *msg2 = creationTime.c_str();
		sendToChannel(channel, msg1, client->getSocket());
		sendMsg(client, channelModeIs);
		sendToChannel(channel, msg2,client->getSocket());
		sendMsg(client, creationTime);
		return ;
	}
}

int		is_itkol(char ch)
{
	if (ch != 'i' && ch != 't' && ch != 'k' && ch != 'o' && ch != 'l')
	{
		return (-1);
	}
	return (1);
}

void	makeArgumentSet(std::vector<std::string> &argumentSet, std::string argument)
{
	std::istringstream	iss(argument);
	std::string			token;

	while (iss >> token)
		argumentSet.push_back(token);
}

void	applyModeToChannel(Server *server, Client *client, modeCommand &modeCommand, Channel &channel)
{
	int	sign = 1;
	std::string mode = modeCommand.mode;
	std::string modeArgument = modeCommand.modeArgument;
	std::string prvModes = channel.getModes();
	std::string	serverName = server->getName();
	std::string unknownError = ERR_UMODEUNKNOWNFLAG(client->getNickName());
	std::vector <std::string> argumentSet;

	if (mode[0] != '+' && mode[0] != '-')
	{
		send(client->getSocket(), unknownError.c_str(), unknownError.length(), 0);
		return ;
	}
	makeArgumentSet(argumentSet, modeArgument);
	for (size_t i = 0; i < mode.length(); i ++)
	{
		char ch = mode[i];
		if (ch == '-' || ch == '+')
		{
			if (ch == '-')
				sign = -1;
			if (ch == '+')
				sign = 1;
		}
		else
		{
			if (is_itkol(ch) == -1)
			{
				std::string msg = ERR_UNKNOWNMODE(client->getNickName(), ch);
				send(client->getSocket(), msg.c_str(), msg.length(), 0);
				channel.setmodes(prvModes);
				return ;
			}
			if (sign == -1)
			{
				if (ch == 'o')
				{
					if (channel.takeOper(modeArgument, client) == -1)
						return ;
				}
				channel.removeFlag(ch);
			}
			if (sign == 1)
			{
				std::vector<Client> clients = server->getClients();
				channel.signPlus(server->getName(), channel, client, argumentSet, ch);
			}
		}
	}
	printModes(client, channel, 1);
}

void	mode(Server *server, const Command &cmd, Client *client)
{
	Channel		*channel;
	modeCommand	parsedModeCommand;
	std::string	channelName = cmd.parameter;
	
	modeParse(cmd, parsedModeCommand);
	channel = server->isChannelExist2(parsedModeCommand.channelName);
	if (channel == 0) // if channel does not exist
	{
		std::string noSuchChannel = ERR_NOSUCHCHANNEL(client->getUserName(), channelName);
		send (client->getSocket(), noSuchChannel.c_str(), noSuchChannel.length(), 0);
		return ;
	}
	if (parsedModeCommand.mode.empty())
	{
		printModes(client, *channel, 0);
		return ;
	}
	else
	{
		if (channel->hasOper(client) == NULL)
		{
			std::string noPrivs = ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getName());
			send (client->getSocket(), noPrivs.c_str(), noPrivs.length(), 0);
			return ;
		}
	}
	applyModeToChannel(server, client, parsedModeCommand, *channel);
}