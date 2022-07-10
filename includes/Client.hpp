/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/07 13:31:18 by aheister      #+#    #+#                 */
/*   Updated: 2022/07/07 14:28:42 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_CLIENT_H
#define WEBSERV_CLIENT_H

#pragma once

#include <netinet/in.h>

class Client {
public:
    Client(void);
    Client(Client const & src);
	Client& operator=(Client const & rhs);
    virtual ~Client(void);

    int                 getClientSocket(void);
    struct sockaddr_in  getClientAddress(void);

    void                setClientSocket(int new_socket);
    void                setClientAddress(struct sockaddr_in new_addr);

private:
    int                 _client_socket;
    struct sockaddr_in 	_client_addr;
};

#endif //WEBSERV_CLIENT_H