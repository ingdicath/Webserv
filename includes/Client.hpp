/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/07 13:31:18 by aheister      #+#    #+#                 */
/*   Updated: 2022/07/20 16:00:00 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netinet/in.h>
#include <sys/time.h>

class Client {
public:
    Client(void);
    Client(Client const & src);
	Client& operator=(Client const & rhs);
    virtual ~Client(void);

    int                 getClientSocket(void) const;
    struct sockaddr_in  getClientAddress(void) const;
    long long           getClientTimeStamp(void) const;

    void                setClientSocket(int new_socket);
    void                setClientAddress(struct sockaddr_in new_addr);
    void                setClientTimeStamp(void);

private:
    int                 _clientSocket;
    struct sockaddr_in 	_clientAddr;
    long long           _clientTimeStamp;
};
