/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 15:09:47 by aheister      #+#    #+#                 */
/*   Updated: 2022/07/25 11:34:24 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "settings.hpp"
#include "Server.hpp"

Server::Server(void) :
	_port(DEFAULT_PORT),
	_host(DEFAULT_HOST),
    _serverName(DEFAULT_SERVER_NAME),
	_errorPage(DEFAULT_ERROR_PAGE),
    _timeOut(DEFAULT_TIMEOUT),
	_serverSocket(-1) {
}

// REMOVE THIS ONE LATER: ONLY FOR TESTING PURPOSES
Server::Server(int port) :
	_port(port),
	_host(DEFAULT_HOST),
    _serverName(DEFAULT_SERVER_NAME),
    _errorPage(DEFAULT_ERROR_PAGE), 
	_timeOut(DEFAULT_TIMEOUT),
	_serverSocket(-1) {
}

Server::Server(const Server & src) {
	*this = src;
}

Server& Server::operator=(const Server & rhs) {
	if (this != & rhs) {
		_port = rhs._port;
		_host = rhs._host;
		_serverName = rhs._serverName;
		_errorPage = rhs._errorPage;
		_timeOut = rhs._timeOut;
		_serverSocket = rhs._serverSocket;
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

const char *Server::setupException::what() const throw() {
	return "Setup failed\n";
}

/*
** DESCRIPTION
** This function does the setup of a server using its _host and _port
** JOBS
** 1. Creates the server_fd (= socket) 
** 2. Makes the socket non-blocking
** 3. Sets the FLAG "REUSE_ADDRESS" on the socket in order to avoid errors in restarting the program 
**    if file descriptors are not closed after exiting.
** 4. Names (binds) the socket
** 5. Creates the connection by putting the server in a listening state
** 6. If either one of these jobs fail, the function returns with EXIT_FAILURE 
*/
int	Server::setupServer(void) {
	int iSetOption = 1;

	try {
		if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			throw setupException();
		}
		if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1) {
			throw setupException();
		}
		setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));
		memset(&_serverAddr, 0, sizeof(_serverAddr));
		_serverAddr.sin_family = AF_INET;
		_serverAddr.sin_port = htons(_port);
		char* host = const_cast<char*>(_host.c_str());	// TODO: put this in configuration part
		_serverAddr.sin_addr.s_addr = inet_addr(host);
		if (bind(_serverSocket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1) {
			throw setupException();
		}
		if (listen(_serverSocket, DEFAULT_BACKLOG) == -1) {
			throw setupException();
		}
	}
	catch (setupException& e) {
		std::cout << RED << e.what();
		std::cout << "Server IP: " << _host << " port: " << _port << std::endl;
		std::cout << "Error: " << strerror(errno) << "\n" << RESET << std::endl;
		if (_serverSocket != -1) 
			close(_serverSocket);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
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
			std::cout << RED "Client " << it->getClientSocket() << " removed" RESET << std::endl; // test: delete later
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
