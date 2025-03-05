/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:25:10 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/25 15:48:12 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"
#include "utils.hpp"

Channel::Channel()
{
	this->_name = "Default";
	this->_channelCreator = 9999;
	this->_isTopic = 0;
	this->_topic = "";
	this->_key = "";
	this->_modes = "+";
	this->_serverCreationTime = time(NULL);
	this->_limit = 9999;
	this->_inviteOnly = 0;
}

Channel::Channel(Client &client, std::string name, int channelCreator)
{
	this->_operators.push_back(client);
	this->_name = name;
	this->_key = "";
	this->_channelCreator = channelCreator;
	this->_topic = "";
	this->_isTopic = 0;
	this->_modes = "+";
	this->_limit = 9999;
	this->_serverCreationTime = time(NULL);
	this->_inviteOnly = 0;
}

Channel::~Channel()
{
}

/* add joiningClient to _joinedClients in Channel object 
  before add, check if client is already exist in joinedClients list. */
void Channel::joinClient(const Client &joiningClient) 
{
	std::string clientName = joiningClient.getUserName();
	for (std::vector<Client>::iterator it = _joinedClients.begin(); it != _joinedClients.end(); ++it)
	{
		if (it->getUserName() == clientName)
			return; // need announce function for both server & client
	}
	_joinedClients.push_back(joiningClient);
}


  	// to remove the client from _joinedClients after KICK command
void Channel::removeClientFromList(std::vector<Client>::iterator clientIt) {
	if (clientIt != _joinedClients.end()) {
		_joinedClients.erase(clientIt);
		std::cout << "debug KICK client from _joinedClients: " << clientIt->getUserName() << this->getName() << std::endl;
	}
}

void Channel::printAllMembers()
{
	std::vector<Client>::iterator it;

	for (; it != _joinedClients.end(); ++it)
	{
		std::cout << it->getUserName() << " " << std::endl;
	}
}


std::string Channel::makeMemberList()
{
	std::vector<Client> memberList = this->getJoinedClients();
	std::string	result;
	
	std::vector<Client>::iterator it = memberList.begin();
	for (; it != memberList.end(); it++)
	{
		result = result + " " + it->getNickName();
	}
	return (result);
}

void Channel::setTopic(std::string topic, std::string whoSet)
{
	this->_topic = topic;
	this->_isTopic = 1;
	this->_whoTopicSet = whoSet;
	this->_whenTopicSet = time(NULL);
}

Client	*Channel::isUserInChannel(std::string nickName)
{
	std::vector<Client>::iterator it = _joinedClients.begin();

	for (; it != _joinedClients.end(); ++it)
	{
		if (it->getNickName() == nickName)
			return &(*it);
	}
	return (NULL);
}

void Channel::clearTopic(std::string whoClear)
{
	this->_topic = "";
	this->_isTopic = 0;
	this->_whoTopicSet = whoClear;
	this->_whenTopicSet = time(NULL);
}

void Channel::removeFlag(char flag)
{
	size_t	pos;

	pos = _modes.find(flag);
	if (pos == std::string::npos)
		return ;
	else
	{
		_modes.erase(pos, 1);
		return ;
	}
}

void Channel::addFlag(char flag)
{
	size_t	pos;
	pos = _modes.find(flag);
	if (pos != std::string::npos)
		return ;
	else
	{
		_modes.push_back(flag);
		return ;
	}
}

void Channel::setmodes(std::string modes)
{
	this->_modes = modes;
}

int	Channel::modeK(std::vector<std::string> &argumentSet)
{
	std::string	argument;

	if (argumentSet.empty() || argumentSet[0].empty())
		return (-1);
	argument = argumentSet[0];
	_key = argument;
	argumentSet.erase(argumentSet.begin());
	return (0);
}

int	Channel::modeO(std::string serverName, Client &client, std::vector<std::string> &argumentSet)
{
	std::string nickName;
	if (argumentSet.empty() || argumentSet[0].empty()) 
		return (-1);
	nickName = argumentSet[0];
	Client *toBeOperator = isUserInChannel(nickName);
	if (toBeOperator == NULL)
	{
		std::string notInChannel = ERR_USERNOTINCHANNEL(serverName, nickName, _name);
		send(client.getSocket(), notInChannel.c_str(), notInChannel.size(), 0);
		return (-1);
	}
	this->_operators.push_back(*toBeOperator);
	argumentSet.erase(argumentSet.begin());
	return (0);
}

int	Channel::modeL(std::string serverName, Client &client, std::vector<std::string> &argumentSet)
{
	size_t		result;
	std::string	argument;

	if (argumentSet.empty() || argumentSet[0].empty())
		return (-1);
	argument = argumentSet[0];
	result = stringToSizeT(argument);
	if (result == 0)
	{
		std::string errMsg = ERR_UNKNOWNMODE(serverName, client.getNickName(), "l");
		send(client.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		return (2);
	}
	_limit = result;
	argumentSet.erase(argumentSet.begin());
	return (0);
}

int Channel::signPlus(std::string serverName, Channel &channel, Client &client, std::vector<std::string> &argumentSet, char ch)
{
	if (ch == 'k')
	{
		if (modeK(argumentSet) == -1)
			return (1);
	}
	if (ch == 'o')
	{
		if (modeO(serverName, client, argumentSet) == -1)
			return (2);
	}
	if (ch == 'l')
	{
		if (modeL(serverName, client, argumentSet) == -1)
			return (3);
	}
	if (ch != 'o')
		channel.addFlag(ch);
	return (0);
}

Client*	Channel::hasOper(Client &client)
{
	std::vector<Client>::iterator it = this->_operators.begin();

	for (; it != this->_operators.end(); it ++)
	{
		if (client.getNickName() == it->getNickName())
			return &(*it);
	}
	return (NULL);
}


// int	Channel::signMinus(Channel &channel, Client &client, std::vector<std::string> &argumentSet, char ch)
// {
	
// }
