/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:04:11 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/25 17:54:32 by junhhong         ###   ########.fr       */
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
	bool									password;
	bool									registered;

	std::string								buffer_in;
	std::string								buffer_out;

	public:
											Client(int socket)
											{
												user_name = "default";
												nick_name = "default_nick";
												clientSocket = socket;
												send(socket, "Please enter the password:\n", 28, 0);
											};
											~Client(){};
	void									setUserName(std::string _name){this->user_name = _name;}
	void									setNickName(std::string _name){this->nick_name = _name;}
	void									setPW(){this->password = true;}
	void									setRegistered(){this->registered = true;}
	void									setSocket(int socket){this->clientSocket = socket;}
	std::string								getUserName() const{return (this->user_name);}
	std::string								getNickName() const{return (this->nick_name);}
	int										getSocket()const{return (this->clientSocket);}
	bool									getRegistered()const{return (this->registered);}
	bool									getPW()const{return (this->password);}
};

#endif