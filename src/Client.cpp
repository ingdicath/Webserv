/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/07 13:40:13 by aheister      #+#    #+#                 */
/*   Updated: 2022/07/08 08:59:18 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
#include <iostream>

Client::Client(void) {
	return;
}

Client::Client(Client const & src) {
	*this = src;
}

Client& Client::operator=(Client const & rhs) {
	if (this != & rhs) {
		_client_socket = rhs._client_socket;
		_client_addr = rhs._client_addr;
	}
	return *this;
}

Client::~Client(void) {
	return;
}

// GET functions
int	Client::getClientSocket(void) {
	return _client_socket;
}
struct sockaddr_in  Client::getClientAddress(void) {
	return _client_addr;
}

// SET functions
void Client::setClientSocket(int new_socket) {
	_client_socket = new_socket;
}

void Client::setClientAddress(struct sockaddr_in new_addr) {
	_client_addr = new_addr;
}