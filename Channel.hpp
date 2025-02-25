/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:22:21 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/24 15:58:52 by junhhong         ###   ########.fr       */
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
	std::vector<Client>						_joinedClients;

	public:
											Channel();
											Channel(std::string name);
											~Channel();
	void									setName(std::string _name){this->_name = _name;}
	void									joinClient(const Client &client);
	std::string								getName()const{return (this->_name);}
	std::vector<Client>&					getJoinedClients(){return (this-> _joinedClients);}
};

#endif