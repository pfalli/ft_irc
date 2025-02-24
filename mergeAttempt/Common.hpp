/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhhong <junhhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:05:43 by junhhong          #+#    #+#             */
/*   Updated: 2025/02/24 15:19:40 by junhhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_HPP
# define COMMON_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>

#include <poll.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>

#include <cerrno>
#include <cstdio>
#include <pthread.h>
#include <sstream>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

#define FAILURE -1
#define POLL_TIMEOUT 0
#define SUCCESS 1
#define DISCONNECT 2
#define IGNORE 5

#endif