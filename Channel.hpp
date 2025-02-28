/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:22:21 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/25 15:47:50 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Common.hpp"
#include "Client.hpp"
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

class Channel {
	private:
	std::string								_name;
	std::string								nameClientList;
	std::string								_topic;
	time_t									_whenTopicSet;
	std::string								_whoTopicSet;
	std::vector<Client>						_joinedClients;
	int										_channelCreator;
	bool									_isTopic;

	public:
											Channel();
											Channel(std::string name, int channelCreator);
											~Channel();
	void									setName(std::string _name){this->_name = _name;}
	void									joinClient(const Client &client);
	bool									getisTopic()const{return (this->_isTopic);}
	std::string								getName()const{return (this->_name);}
	std::string								getTopic()const{return (this->_topic);}
	time_t									getwhenTopicSet()const{return (this->_whenTopicSet);}
	std::string								getwhoTopicSet()const{return (this->_whoTopicSet);}
	std::vector<Client>&					getJoinedClients(){return (this-> _joinedClients);}
	void									printAllMembers();
	std::string 							makeMemberList();


	void removeClientFromList(std::vector<Client>::iterator person);
};

#endif