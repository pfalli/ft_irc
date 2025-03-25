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
	std::string								real_name;
	std::string								nick_name;
	bool									password;
	bool									user;
	bool									nick;
	bool									registered;
	bool									regstr_msg_sent;

	std::string								buffer_in;
	std::string								buffer_out;

	public:
											Client()
											{
												user_name = "no_user_set";
												nick_name = "no_nick_set";
												real_name = "no_real_set";
												password = false;
												user = false;
												nick = false;
												registered =false;
												clientSocket = 4;
												regstr_msg_sent = false;
											};
											Client(int socket)
											{
												user_name = "no_user_set";
												nick_name = "no_nick_set";
												real_name = "no_real_set";
												password = false;
												user = false;
												nick = false;
												registered =false;
												clientSocket = socket;
												regstr_msg_sent = false;
											};
											~Client(){};
	void									setUserName(std::string _name){this->user_name = _name;}
	void									setNickName(std::string _name){this->nick_name = _name;}
	void									setRealName(std::string _name){this->real_name = _name;}
	void									setRegistered(){this->registered = true;}
	void									setSocket(int socket){this->clientSocket = socket;}
	void									setPW(){this->password = true;}
	void									setUser(){this->user = true;}
	void									setNick(){this->nick = true;}
	void									setRGSTR_MSG(){this->regstr_msg_sent = true;}

	std::string								getUserName() const{return (this->user_name);}
	std::string								getNickName() const{return (this->nick_name);}
	std::string								getRealName() const{return (this->real_name);}
	int										getSocket()const{return (this->clientSocket);}
	bool									getRGSTR_MSG()const{return(this->regstr_msg_sent);}
	bool									getRegistered()const{return (this->registered);}
	bool									getPW()const{return (this->password);}
	bool									getUser()const{return (this->user);}
	bool									getNick()const{return (this->nick);}
};

#endif