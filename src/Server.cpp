
#include "../includes/settings.hpp"
#include "../includes/Server.hpp"

#include <iostream>

Server::Server(void) :
	_port(DEFAULT_PORT),
    _server_name(DEFAULT_SERVER_NAME),
    _error_page(DEFAULT_ERROR_PAGE) {

	std::cout << "Server created" << std::endl;
}

Server::Server(const Server & src) {
	*this = src;
}

Server& Server::operator=(const Server & rhs) {
	if (this != & rhs) {
		_port = rhs._port;
		_server_name = rhs._server_name;
		_error_page = rhs._error_page;
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
	if ((this->_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw (std::runtime_error("Socket creation failed"));
    if (fcntl(this->_server_socket, F_SETFL, O_NONBLOCK) < 0)
		throw (std::runtime_error("Transforming the socket to non-blocking failed"));
	
	memset(&_server_addr, 0, sizeof(_server_addr));
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port);
    _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(this->_server_socket, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
		throw (std::runtime_error("Binding of the socket failed"));

	if (listen(this->_server_socket, DEFAULT_BACKLOG) < 0)
		throw (std::runtime_error("Listen failed"));

	//test
	std::cout << "Socket listening" << std::endl;
}

/*
** Accepts an incoming connection from a client. In the struct _client_address, the IP address and port of the client is stored.
*/
int Server::acceptConnection(void) {
	Client	client;
	int		new_socket;
	struct	sockaddr_in client_addr;
	int		client_addrlen = sizeof(client_addr);

	if ((new_socket = accept(this->_server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&client_addrlen)) < 0) {
		throw (std::runtime_error("Accept incoming connection failed"));
	}

	client.setClientAddress(client_addr);
	// test
	std::cout << "Client: " << inet_ntoa(client_addr.sin_addr) << std::endl;
	client.setClientSocket(new_socket);
	_clients.push_back(client);

	// test
	std::cout << "Client accepted: " << new_socket << std::endl;

	return new_socket;
}

int		Server::getServerSocket(void) {
	return _server_socket;
}

int		Server::getPort(void) {
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
