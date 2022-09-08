/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 15:09:47 by aheister      #+#    #+#                 */
/*   Updated: 2022/08/30 11:16:55 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <cerrno>
#include <map>
#include "Server.hpp"
#include "../http/Response.hpp"
#include "../configs/Parser.hpp"
#include <algorithm> //testing purpose, delete
#include <iostream> //testing purpose, delete
#include <iterator> //testing purpose, delete

Server::Server(void) :
		_port(DEFAULT_PORT),
		_host(DEFAULT_HOST),
		_clientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE),
		_timeOut(DEFAULT_TIMEOUT),
		_serverSocket(-1) {
	_serverName.push_back(DEFAULT_SERVER_NAME);
	_setDefaultErrorPages();
	_flagPort = false;
	_flagHost = false;
}

// REMOVE THIS ONE LATER: ONLY FOR TESTING PURPOSES
Server::Server(int port) :
		_port(port),
		_host(DEFAULT_HOST),
		_timeOut(DEFAULT_TIMEOUT),
		_serverSocket(-1) {
}

Server::Server(const Server &src) {
	*this = src;
}

Server &Server::operator=(const Server &rhs) {
	if (this != &rhs) {
		_port = rhs._port;
		_host = rhs._host;
		_serverName = rhs._serverName;
		_index = rhs._index;
		_errorPage = rhs._errorPage;
		_clientMaxBodySize = rhs._clientMaxBodySize;
		_locations = rhs._locations;
		_timeOut = rhs._timeOut;
		_serverSocket = rhs._serverSocket;
	}
	return *this;
}

Server::~Server(void) {
	return;
}

const char *Server::setupException::what() const throw() {
	return "Setup failed\n";
}

/*
** Configuration of the server 
*/
void Server::configServer(void) {
	return;
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
int Server::setupServer(void) {
	int iSetOption = 1;

	try {
		if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			throw setupException();
		}
		if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1) {
			throw setupException();
		}
		setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &iSetOption, sizeof(iSetOption));
		memset(&_serverAddr, 0, sizeof(_serverAddr));
		_serverAddr.sin_family = AF_INET;
		_serverAddr.sin_port = htons(_port);
		char *host = const_cast<char *>(_host.c_str());    // TODO: put this in configuration part
		_serverAddr.sin_addr.s_addr = inet_addr(host);
		if (bind(_serverSocket, (struct sockaddr *) &_serverAddr, sizeof(_serverAddr)) == -1) {
			throw setupException();
		}
		if (listen(_serverSocket, DEFAULT_BACKLOG) == -1) {
			throw setupException();
		}
	}
	catch (setupException &e) {
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
** DESCRIPTION
** Function that accepts an incoming connection from a client. In the struct _client_address, 
** the IP address and port of the client is stored.
*/
int Server::acceptConnection(void) {

	int newSocket;
	struct sockaddr_in clientAddr;
	int clientAddrlen = sizeof(clientAddr);

	if ((newSocket = accept(this->_serverSocket, (struct sockaddr *) &clientAddr, (socklen_t *) &clientAddrlen)) ==
		-1) {
		throw (std::runtime_error("Accept incoming connection failed"));
	} else {
		this->addClient(newSocket, clientAddr);
        _requests.insert(std::make_pair(newSocket, "")); //lin testing
	}
	return newSocket;
}

/*
** DESCRIPTION
** Function to create a new client object and add it to the back of the vector of clients of a server
*/
void Server::addClient(int newSocket, struct sockaddr_in clientAddr) {
	Client client;

	client.setClientAddress(clientAddr);
	std::cout << "Client: " << inet_ntoa(clientAddr.sin_addr) << std::endl; // test: delete later
	client.setClientSocket(newSocket);
	client.setClientTimeStamp();
	_clients.push_back(client);
	std::cout << "Client accepted: " << newSocket << std::endl; // test: delete later
}

/*
** DESCRIPTION
** Function to remove a client from the vector of clients of a server
*/
void Server::removeClient(int thisSocket) {
	for (std::vector<Client>::iterator it = _clients.begin(); it < _clients.end(); it++) {
		if (it->getClientSocket() == thisSocket) {
			std::cout << RED "Client " << it->getClientSocket() << " removed" RESET << std::endl; // test: delete later
			close(it->getClientSocket());
			_clients.erase(it);
            _requests.erase(thisSocket);
		}
	}
}

/**
 * Getters
 */

int Server::getPort(void) const {
	return _port;
}
const std::string &Server::getHost() const {
	return _host;
}

std::vector<Location> *Server::getLocations(void) {
	return &_locations;
}

const std::vector<std::string> &Server::getServerName() const {
	return _serverName;
}

const std::string &Server::getIndex() const {
	return _index;
}

long Server::getClientMaxBodySize() const {
	return _clientMaxBodySize;
}

const std::map<int, std::string> &Server::getErrorPage() const {
	return _errorPage;
}

int Server::getServerSocket(void) const {
	return _serverSocket;
}

std::vector<Client> Server::getClients(void) const {
	return _clients;
}

long long Server::getTimeout(void) const {
	return _timeOut;
}

/**
 * Setters
 */

void Server::setPort(int port) {
	if (_flagPort) {
		throw Parser::ConfigFileException("Duplicate value in 'port'.");
	}
	_port = port;
	_flagPort = true;
}

void Server::setHost(std::string host) {
	if (_flagHost) {
		throw Parser::ConfigFileException("Duplicate value in 'host'.");
	}
	_host = host;
	_flagHost = true;
}

void Server::setServerName(const std::vector<std::string> &serverName) {
	_serverName = serverName;
}

void Server::setClientMaxBodySize(long clientMaxBodySize) {
	_clientMaxBodySize = clientMaxBodySize;
}

void Server::_setDefaultErrorPages() {
	_errorPage.insert(std::pair<int, std::string>(400, "/errors/400.html"));
	_errorPage.insert(std::pair<int, std::string>(403, "/errors/403.html"));
	_errorPage.insert(std::pair<int, std::string>(404, "/errors/404.html"));
	_errorPage.insert(std::pair<int, std::string>(405, "/errors/405.html"));
	_errorPage.insert(std::pair<int, std::string>(408, "/errors/408.html"));
	_errorPage.insert(std::pair<int, std::string>(410, "/errors/410.html"));
	_errorPage.insert(std::pair<int, std::string>(411, "/errors/411.html"));
	_errorPage.insert(std::pair<int, std::string>(413, "/errors/413.html"));
	_errorPage.insert(std::pair<int, std::string>(414, "/errors/414.html"));
	_errorPage.insert(std::pair<int, std::string>(418, "/errors/418.html"));
	_errorPage.insert(std::pair<int, std::string>(429, "/errors/429.html"));
	_errorPage.insert(std::pair<int, std::string>(431, "/errors/431.html"));
	_errorPage.insert(std::pair<int, std::string>(500, "/errors/500.html"));
	_errorPage.insert(std::pair<int, std::string>(501, "/errors/501.html"));
	_errorPage.insert(std::pair<int, std::string>(505, "/errors/505.html"));
}

/**
 * If the status error code already exists in the default error Pages map, the path to this error
 * is updated, if is not found, it is added to the current map of error pages.
 * @param errorPage Is the pair of status error code and path defined in the config file
 * by the user.
 */
void Server::addErrorPage(const std::pair<int, std::string> &errorPage) {
	std::map<int, std::string>::iterator it = _errorPage.find(errorPage.first);
	if (it != _errorPage.end()) {
		_errorPage.at(it->first) = errorPage.second;
	} else {
		_errorPage.insert(errorPage);
	}
}

void Server::addLocation(Location location) {
	_locations.push_back(location);
}

//lin testing
static int chunkedEnd(const std::string &str){
    std::string endStr = "0\r\n\r\n";
    size_t i = str.size();
    size_t j = endStr.size();

    while (j > 0) {
        i--;
        j--;
        if (i < 0 || str[i] != endStr[j]) {
            return (1);
        }
    }
    return (0);
}

//return 0 is finished, return -1 when closing the conncetion
int     Server::recvRequest(int clientId) {
    char    buffer[MAXLINE + 1] = {0};
    int     ret;

    ret = recv(clientId, buffer, MAXLINE - 1, 0);
    if (ret == 0) {
        std::cout << "Connection was closed by client." << std::endl;
        return EXIT_FAILURE;
    }
    else if (ret == -1) {
        std::cout << "recv error, closing connection" << std::endl;
        return EXIT_FAILURE;
    }

    _requests[clientId] += std::string(buffer);

    size_t i = _requests[clientId].find("\r\n\r\n"); // find the end of the headers
    if (i != std::string::npos) { //there is a body
        if (_requests[clientId].find("Content-Length: ") == std::string::npos) { //no info about content length
            if (_requests[clientId].find("Transfer-Encoding: chunked") != std::string::npos) { //it is chunked encoding
                if (chunkedEnd(_requests[clientId]) == 0) { //chunked finished
                    return EXIT_SUCCESS;
                } else {
                    return -1; //chunked not finished
                }
            }
            else { //no content length and not chunked encoding
                return EXIT_SUCCESS;
            }
        }
        else { //there is content length
            size_t	len = std::atoi(_requests[clientId].substr(_requests[clientId].find("Content-Length: ") + 16, 10).c_str());
            if (_requests[clientId].size() >= len + i + 4) {
                return EXIT_SUCCESS;
            } else {
                return -1; //content length does not match the body size
            }
        }
    }
    return -1;
}

void    Server::processChunk(int clientId) {
    std::string heads = _requests[clientId].substr(0, _requests[clientId].find("\r\n\r\n"));
    std::string	chunks = _requests[clientId].substr(_requests[clientId].find("\r\n\r\n") + 4, _requests[clientId].size() - 1);
    std::string subChunk = chunks.substr(0,100);
    std::string body = "";
    int chunkSize = strtol(subChunk.c_str(), NULL, 16);
    size_t i = 0;

    while(chunkSize) {
        i = chunks.find("\r\n", i) + 2;
        body += chunks.substr(i, chunkSize);
        i += chunkSize + 2;
        subChunk = chunks.substr(i, 100);
        chunkSize = strtol(subChunk.c_str(), NULL, 16);
    }
    _requests[clientId] = heads + "\r\n\r\n" + body + "\r\n\r\n";
}

void    Server::processRequest(int clientId) {
    std::string recvd = "";

    if (_requests[clientId].find("Transfer-Encoding: chunked") != std::string::npos &&
            _requests[clientId].find("Transfer-Encoding: chunked") < _requests[clientId].find("\r\n\r\n")) {
        processChunk(clientId);
    }

    //output for testing
    if (_requests[clientId].size() < 1000) {
        std::cout << "\n_request:\n" << _requests[clientId] << std::endl;
    } else {
        std::cout << "\n_request:\n" << _requests[clientId].substr(0, 1000) << std::endl;
    }
    _requests.erase(clientId);
}
