/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 15:09:47 by aheister      #+#    #+#                 */
/*   Updated: 2022/07/14 08:57:47 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "settings.hpp"
#include "Server.hpp"

#include <iostream>

Server::Server(void) :
	_port(DEFAULT_PORT),
    _serverName(DEFAULT_SERVER_NAME),
    _errorPage(DEFAULT_ERROR_PAGE) {

	std::cout << "Server created" << std::endl;
}

Server::Server(const Server & src) {
	*this = src;
}

Server& Server::operator=(const Server & rhs) {
	if (this != & rhs) {
		_port = rhs._port;
		_serverName = rhs._serverName;
		_errorPage = rhs._errorPage;
	}
	return *this;
}

Server::~Server(void) {
	return;
}

/*
** Configuration of the server 
*/
void	Server::configServer(void) {
	return;
}

/*
** 1. Creates the server_fd (= socket) and makes it non-blocking
** 2. Names (binds) the socket
** 3. Creates the connection by putting the server in a listening state
*/
void	Server::setupServer(void) {
	if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw (std::runtime_error("Socket creation failed"));
    if (fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK) < 0)
		throw (std::runtime_error("Transforming the socket to non-blocking failed"));
	
	memset(&_serverAddr, 0, sizeof(_serverAddr));
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(_port);
    _serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(this->_serverSocket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0)
		throw (std::runtime_error("Binding of the socket failed"));

	if (listen(this->_serverSocket, DEFAULT_BACKLOG) < 0)
		throw (std::runtime_error("Listen failed"));

	//test
	std::cout << "Socket listening" << std::endl;
}

/*
** Accepts an incoming connection from a client. In the struct _client_address, the IP address and port of the client is stored.
*/
int Server::acceptConnection(void) {
	Client	client;
	int		newSocket;
	struct	sockaddr_in clientAddr;
	int		clientAddrlen = sizeof(clientAddr);

	if ((newSocket = accept(this->_serverSocket, (struct sockaddr *)&clientAddr, (socklen_t*)&clientAddrlen)) < 0) {
		throw (std::runtime_error("Accept incoming connection failed"));
	}

	client.setClientAddress(clientAddr);
	// test
	std::cout << "Client: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
	client.setClientSocket(newSocket);
	_clients.push_back(client);

	// test
	std::cout << "Client accepted: " << newSocket << std::endl;

	return newSocket;
}


/*
** GET functions
*/
int		Server::getServerSocket(void) const {
	return _serverSocket;
}

int		Server::getPort(void) const {
	return _port;
}








// const std::vector<std::string> &Server::getServerName() const {
// 	return _server_name;
// }

// Server::Server(Server *pServer) {
// }

// void Server::validateAndSetListen(std::vector<std::string> values) {
// 	if (Configurator::_isValidListenValues(values)) {
// 		std::set<std::string> mySet;
// 		for (size_t i = 0; i < values.size(); i++) {
// 			mySet.insert(values[i]);
// 		}
// 		_listen = mySet;
// 	}
// }

// const std::vector<Location> &Server::getLocations() const {
// 	return _locations;
// }

// const std::set<std::string> &Server::getListen() const {
// 	return _listen;
// }

// const std::map<std::size_t, std::string> &Server::getErrorPage() const {
// 	return _error_page;
// }

// size_t Server::getClientMaxBodySize() const {
// 	return _client_max_body_size;
// }

// void Server::setLocations(const std::vector<Location> &locations) {
// 	_locations = locations;
// }

// void Server::setListen1(const std::set<std::string> &listen) {
// 	_listen = listen;
// }

// void Server::setServerName(const std::vector<std::string> &serverName) {
// 	_server_name = serverName;
// }

// void Server::setErrorPage(const std::map<std::size_t, std::string> &errorPage) {
// 	_error_page = errorPage;
// }

// void Server::setClientMaxBodySize(size_t clientMaxBodySize) {
// 	_client_max_body_size = clientMaxBodySize;
// }


//
//void Server::setListenFromInput(const std::string &str) {
//
//
//
//}


//// https://stackoverflow.com/questions/18677171/throwing-exception-when-the-same-key-inserted-into-stdmap
//void Server::setListen(const std::string& str) {
//	if (!_listen.insert(Server::_isValidPortRange(str)).second){
//		//check if is better create a specific class for every exception
//		throw std::invalid_argument("Config error: duplicate " + str);
//	}
//}
