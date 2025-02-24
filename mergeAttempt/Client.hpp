/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:04:11 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/24 15:19:15 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
#include "Common.hpp"

class Client
{
	private:
	int										clientSocket; //same as client_fd
	std::string								user_name;
	std::string								nick_name;

	public:
											Client(){user_name = "default"; nick_name = "default_nick";}
											~Client(){};
	void									setUserName(std::string _name){this->user_name = _name;}
	void									setNickName(std::string _name){this->nick_name = _name;}
	void									setSocket(int socket){this->clientSocket = socket;}
	std::string								getUserName() const{return (this->user_name);}
	std::string								getNickName() const{return (this->nick_name);}
	int										getSocket()const{return (this->clientSocket);}
};

#endif