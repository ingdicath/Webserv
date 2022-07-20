/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 15:09:47 by aheister      #+#    #+#                 */
/*   Updated: 2022/07/20 16:44:51 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "settings.hpp"
#include "Server.hpp"

Server::Server(void) :
	_port(DEFAULT_PORT),
    _serverName(DEFAULT_SERVER_NAME),
	_errorPage(DEFAULT_ERROR_PAGE),
    _timeOut(DEFAULT_TIMEOUT) {

	std::cout << "Server created" << std::endl;
}

Server::Server(int port) :
	_port(port),
    _serverName(DEFAULT_SERVER_NAME),
    _errorPage(DEFAULT_ERROR_PAGE), 
	_timeOut(DEFAULT_TIMEOUT) {

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
		_timeOut = rhs._timeOut;
	}
	return *this;
}

Server::~Server(void) {
	return;
}

/*
** Configuration of the server 
*/
void	Server::configServer(int port) {
	return;
}

/*
** 1. Creates the server_fd (= socket) and makes it non-blocking
** 2. Names (binds) the socket
** 3. Creates the connection by putting the server in a listening state
*/
void	Server::setupServer(void) {
	int iSetOption = 1;
	if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw (std::runtime_error("Socket creation failed"));
    if (fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK) < 0)
		throw (std::runtime_error("Transforming the socket to non-blocking failed"));
	// this function sets the sock option "Reuse address" so if a file descriptor is not closed after exiting, 
	// you can still restart the program without errors
	setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));
	memset(&_serverAddr, 0, sizeof(_serverAddr));
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(_port);
    _serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(this->_serverSocket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1) {
		std::cout << this->_serverSocket << " " << strerror(errno) << std::endl;
		throw (std::runtime_error("Binding of the socket failed"));
	}
	if (listen(this->_serverSocket, DEFAULT_BACKLOG) < 0)
		throw (std::runtime_error("Listen failed"));

	//test
	std::cout << "Socket listening" << std::endl;
}

/*
** Accepts an incoming connection from a client. In the struct _client_address, the IP address and port of the client is stored.
*/
int Server::acceptConnection(void) {

	int		newSocket;
	struct	sockaddr_in clientAddr;
	int		clientAddrlen = sizeof(clientAddr);

	if ((newSocket = accept(this->_serverSocket, (struct sockaddr *)&clientAddr, (socklen_t*)&clientAddrlen)) == -1) {
		throw (std::runtime_error("Accept incoming connection failed"));
	}
	else {
		this->addClient(newSocket, clientAddr);
	}
	return newSocket;
}

void	Server::addClient(int newSocket, struct	sockaddr_in clientAddr) {
	Client	client;

	client.setClientAddress(clientAddr);
	std::cout << "Client: " << inet_ntoa(clientAddr.sin_addr) << std::endl; // test: delete later
	client.setClientSocket(newSocket);
	client.setClientTimeStamp();
	_clients.push_back(client);
	std::cout << "Client accepted: " << newSocket << std::endl; // test: delete later
}

void	Server::removeClient(int thisSocket) {
	for (std::vector<Client>::iterator it = _clients.begin(); it < _clients.end(); it++) {
		if (it->getClientSocket() == thisSocket) {
			std::cout << "Client " << it->getClientSocket() << " removed" << std::endl; // test: delete later
			close(it->getClientSocket());
			_clients.erase(it);
		}
	}
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

std::vector<Client>	Server::getClients(void) const {
	return _clients;
}

long long Server::getTimeout(void) const {
	return _timeOut;
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
