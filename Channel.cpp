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

Channel::Channel()
{
	this->_name = "Default";
	this->_channelCreator = 9999;
	this->_isTopic = 0;
	this->_modes = "+";
	this->_serverCreationTime = time(NULL);
}

Channel::Channel(std::string name, int channelCreator)
{
	this->_name = name;
	this->_channelCreator = channelCreator;
	this->_topic = "No topic set";
	this->_isTopic = 0;
	this->_modes = "+";
	this->_serverCreationTime = time(NULL);
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

void Channel::clearTopic(std::string whoClear)
{
	this->_topic = "";
	this->_isTopic = 0;
	this->_whoTopicSet = whoClear;
	this->_whenTopicSet = time(NULL);
}