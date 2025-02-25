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
	}

	Channel::Channel(std::string name, int channelCreator)
	{
		this->_name = name;
		this->_channelCreator = channelCreator;
		this->_topic = "No topic set";
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