/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/07 13:31:18 by aheister      #+#    #+#                 */
/*   Updated: 2022/09/12 11:24:06 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <netinet/in.h>
#include <sys/time.h>
#include "../utils/settings.hpp"

class Client {
public:
    Client(void);
    Client(Client const & src);
	Client& operator=(Client const & rhs);
    virtual ~Client(void);

    int                 getClientSocket(void) const;
    uint32_t            getClientTimeStamp(void) const;

    void                setClientSocket(int new_socket);
    void                setClientAddress(struct sockaddr_in new_addr);
    void                setClientTimeStamp(void);

private:
    int                 _clientSocket;
    struct sockaddr_in 	_clientAddr;
    uint32_t			_clientTimeStamp;
};
