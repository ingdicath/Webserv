/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/07 13:40:13 by aheister      #+#    #+#                 */
/*   Updated: 2022/09/12 11:24:10 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) {
	return;
}

Client::Client(Client const & src) {
	*this = src;
}

Client& Client::operator=(Client const & rhs) {
	if (this != & rhs) {
		_clientSocket = rhs._clientSocket;
		_clientAddr = rhs._clientAddr;
		_clientTimeStamp = rhs._clientTimeStamp;
	}
	return *this;
}

Client::~Client(void) {
	return;
}

/*
** GET functions
*/
int	Client::getClientSocket(void) const {
	return _clientSocket;
}

uint32_t Client::getClientTimeStamp(void) const {
	return _clientTimeStamp;
}

/*
** SET functions
*/
void Client::setClientSocket(const int newSocket) {
	_clientSocket = newSocket;
}

void Client::setClientAddress(struct sockaddr_in newAddr) {
	_clientAddr = newAddr;
}

void Client::setClientTimeStamp(void) {
	struct	timeval tv;

	gettimeofday(&tv, NULL);
	_clientTimeStamp = tv.tv_sec;
}
