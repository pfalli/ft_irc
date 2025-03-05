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
	time_t									_serverCreationTime;
	std::string								_whoTopicSet;
	std::string								_modes;
	std::string								_modeArguments;
	std::string								_key;
	std::vector<Client>						_joinedClients;
	std::vector<Client>						_operators;
	size_t									_limit;
	int										_channelCreator;
	bool									_isTopic;
	bool									_inviteOnly;

	public:
											Channel();
											Channel(Client &client, std::string name, int channelCreator);
											~Channel();
	void									setName(std::string _name){this->_name = _name;}
	void									joinClient(const Client &client);
	void									setisTopic(bool num){this->_isTopic = num;}
	bool									getisTopic()const{return (this->_isTopic);}
	std::string								getName()const{return (this->_name);}
	std::string								getTopic()const{return (this->_topic);}
	time_t									getwhenTopicSet()const{return (this->_whenTopicSet);}
	time_t									getserverCreationTime()const{return (this->_serverCreationTime);}
	std::string								getwhoTopicSet()const{return (this->_whoTopicSet);}
	std::string								getmodeArguments()const{return (this->_modeArguments);}
	std::string								getModes()const{return (this->_modes);}
	std::vector<Client>&					getJoinedClients(){return (this-> _joinedClients);}
	void									printAllMembers();
	std::string 							makeMemberList();
	void									setTopic(std::string topic, std::string whoSet);
	void									clearTopic(std::string whoClear);
	void									removeFlag(char flag);
	void									addFlag(char flag);
	void									setmodes(std::string modes);
	int										signPlus(std::string serverName, Channel &channel, Client &client, std::vector<std::string> &argumentSet, char ch);
	int										signMinus(Channel &channel, Client &client, std::vector<std::string> &argumentSet, char ch);
	Client									*isUserInChannel(std::string nickName);
	size_t									getLimit()const{return (this->_limit);}

	int										modeK(std::vector<std::string> &argumentSet);
	int										modeO(std::string serverName, Client &client, std::vector<std::string> &argumentSet);
	int										modeL(std::string serverName, Client &client, std::vector<std::string> &argumentSet);
	Client*									hasOper(Client &client);
	void									removeClientFromList(std::vector<Client>::iterator person);

	// piero added
	std::vector<Client>&					getOperators(){return (this->_operators);}
};

#endif