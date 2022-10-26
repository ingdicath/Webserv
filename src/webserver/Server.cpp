/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 15:09:47 by aheister      #+#    #+#                 */
/*   Updated: 2022/10/23 17:24:13 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <cerrno>
#include <map>
#include "Server.hpp"
#include "../configs/Parser.hpp"

Server::Server(void) :
		_port(DEFAULT_PORT),
		_host(DEFAULT_HOST),
		_clientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE),
		_timeOut(DEFAULT_TIMEOUT),
		_serverSocket(-1) {
	_serverName.push_back(DEFAULT_SERVER_NAME);
	setDefaultErrorPages();
	_flagPort = false;
	_flagHost = false;
}

Server::Server(const Server &src) : _port(), _clientMaxBodySize(),
									_flagPort(), _flagHost(), _timeOut(),
									_serverSocket(), _serverAddr() {
	*this = src;
}

Server &Server::operator=(const Server &rhs) {
	if (this != &rhs) {
		_port = rhs._port;
		_host = rhs._host;
		_serverName = rhs._serverName;
		_errorPage = rhs._errorPage;
		_clientMaxBodySize = rhs._clientMaxBodySize;
		_locations = rhs._locations;
		_relatedServers = rhs._relatedServers;
		_serverNameSet = rhs._serverNameSet;
		_timeOut = rhs._timeOut;
		_serverSocket = rhs._serverSocket;
	}
	return *this;
}

Server::~Server(void) {
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
int Server::setupServer(void) {
	int iSetOption = 1;

	try {
		if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			throw setupException();
		}
		if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1) {
			throw setupException();
		}
		if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &iSetOption, sizeof(iSetOption)) == -1) {
			throw setupException();
		}
		memset(&_serverAddr, 0, sizeof(_serverAddr));
		_serverAddr.sin_family = AF_INET;
		_serverAddr.sin_port = htons(_port);
		_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
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
	struct sockaddr_in clientAddr = {};
	int clientAddrlen = sizeof(clientAddr);

	if ((newSocket = accept(this->_serverSocket, (struct sockaddr *) &clientAddr, (socklen_t *) &clientAddrlen)) ==
		-1) {
		throw (std::runtime_error("Accept incoming connection failed"));
	} else {
		this->addClient(newSocket, clientAddr);
		_requests.insert(std::make_pair(newSocket, ""));
		_requestsHeader.insert(std::make_pair(newSocket, ""));
		_ret.insert(std::make_pair(newSocket, 200));
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
	client.setClientSocket(newSocket);
	client.setClientTimeStamp();
	_clients.push_back(client);
}

/*
** DESCRIPTION
** Function to remove a client from the vector of clients of a server
*/
void Server::removeClient(int thisSocket) {
	for (std::vector<Client>::iterator it = _clients.begin(); it < _clients.end(); it++) {
		if (it->getClientSocket() == thisSocket) {
			close(it->getClientSocket());
			_clients.erase(it);
			_requests.erase(thisSocket);
			_responses.erase(thisSocket); // testing stuff
		}
	}
}

/*
** Getters
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

unsigned long Server::getClientMaxBodySize() const {
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

uint32_t Server::getTimeout(void) const {
	return _timeOut;
}

/*
** Setters
*/

void Server::setPort(int port) {
	if (_flagPort) {
		throw Parser::ConfigFileException("'port' directive is duplicated.");
	}
	_port = port;
	_flagPort = true;
}

void Server::setHost(std::string host) {
	if (_flagHost) {
		throw Parser::ConfigFileException("'host' directive is duplicated.");
	}
	_host = host;
	_flagHost = true;
}

void Server::setServerName(const std::vector<std::string> &serverName) {
	_serverName = serverName;
	_serverNameSet.insert(serverName.begin(),serverName.end());
}

void Server::setClientMaxBodySize(unsigned long clientMaxBodySize) {
	_clientMaxBodySize = clientMaxBodySize;
}

void Server::setDefaultErrorPages() {
	_errorPage.insert(std::pair<int, std::string>(400, "/errors/400.html"));
	_errorPage.insert(std::pair<int, std::string>(403, "/errors/403.html"));
	_errorPage.insert(std::pair<int, std::string>(404, "/errors/404.html"));
	_errorPage.insert(std::pair<int, std::string>(405, "/errors/405.html"));
	_errorPage.insert(std::pair<int, std::string>(408, "/errors/408.html"));
	_errorPage.insert(std::pair<int, std::string>(410, "/errors/410.html"));
	_errorPage.insert(std::pair<int, std::string>(411, "/errors/411.html"));
	_errorPage.insert(std::pair<int, std::string>(413, "/errors/413.html"));
	_errorPage.insert(std::pair<int, std::string>(414, "/errors/414.html"));
	_errorPage.insert(std::pair<int, std::string>(415, "/errors/415.html"));
	_errorPage.insert(std::pair<int, std::string>(418, "/errors/418.html"));
	_errorPage.insert(std::pair<int, std::string>(429, "/errors/429.html"));
	_errorPage.insert(std::pair<int, std::string>(431, "/errors/431.html"));
	_errorPage.insert(std::pair<int, std::string>(500, "/errors/500.html"));
	_errorPage.insert(std::pair<int, std::string>(501, "/errors/501.html"));
	_errorPage.insert(std::pair<int, std::string>(502, "/errors/502.html"));
	_errorPage.insert(std::pair<int, std::string>(505, "/errors/505.html"));
}

/*
** If the status error code already exists in the default error Pages map, the path to this error
** is updated, if is not found, it is added to the current map of error pages.
** @param errorPage Is the pair of status error code and path defined in the config file
** by the user.
*/
void Server::addErrorPage(const std::pair<int, std::string> &errorPage) {
	std::map<int, std::string>::iterator it = _errorPage.find(errorPage.first);
	if (it != _errorPage.end()) {
		_errorPage.at(it->first) = errorPage.second;
	} else {
		_errorPage.insert(errorPage);
	}
}

/*
** Before add the location block to the current server, it checks if there is
** a duplicate path location. In affirmative case, it throws an error.
*/
void Server::addLocation(Location location) {
	checkDuplicateLocationPath(location.getPathLocation());
	_locations.push_back(location);
}


static int chunkedEnd(const std::string &str) {
	std::string endStr = "0\r\n\r\n";
	int i = static_cast<int>(str.size());
	int j = static_cast<int>(endStr.size());

	while (j > 0) {
		i--;
		j--;
		if (i < 0 || str[i] != endStr[j]) {
			return (1);
		}
	}
	return (0);
}

//return 0 is finished, return -1 when closing the connection
int Server::recvRequest(int socket) {
	char buffer[MAXLINE + 1] = {0};
	int ret;

	ret = recv(socket, buffer, MAXLINE - 1, 0);
	if (ret == 0) {
		std::cout << "Connection was closed by client." << std::endl;
		return EXIT_FAILURE;
	} else if (ret == -1) {
		std::cout << "recv error, closing connection" << std::endl;
		return EXIT_FAILURE;
	}
	_requests[socket] += std::string(buffer);
	size_t endOfHeader = _requests[socket].find("\r\n\r\n"); // find the end of the headers
	if (endOfHeader != std::string::npos) {
		endOfHeader += 4;
	} else {
		return -1;
	}
	if (endOfHeader != std::string::npos && _requestsHeader[socket].empty()) {
		_requestsHeader[socket] = _requests[socket].substr(0, endOfHeader);
	}
	if (!_requestsHeader[socket].empty()) {
		if (_requestsHeader[socket].find("Content-Type: multipart/form-data") != std::string::npos) { // processing multipart/formdata
			unsigned long pos = _requestsHeader[socket].find("boundary=----WebKitFormBoundary") + 31;
			std::string boundaryID = _requests[socket].substr(pos , 16);
			std::string boundaryStart = "------WebKitFormBoundary" + boundaryID;
			std::string boundaryEnd = "------WebKitFormBoundary" + boundaryID + "--";
			if (_requests[socket].find(boundaryStart) != std::string::npos) {
				if (_requests[socket].find("Content-Type: text/plain") == std::string::npos && 
						_requests[socket].find("Content-Type: application/octet-stream") == std::string::npos) {
					_ret[socket] = 415;
					return EXIT_SUCCESS;
				}
			}
			if (_requests[socket].find(boundaryEnd) != std::string::npos) {
				return EXIT_SUCCESS;
			} else {
				return -1; // multipart/formdata not finished
			}
		}
		else if (_requestsHeader[socket].find("Transfer-Encoding: chunked") != std::string::npos) { //it is chunked encoding
			if (chunkedEnd(_requests[socket]) == 0) { //chunked finished
				return EXIT_SUCCESS;
			} else {
				return -1; //chunked not finished
			}
		}
		else if (_requestsHeader[socket].find("Content-Length: ") != std::string::npos) { //there is content length
			size_t len = std::atoi(_requests[socket].substr(_requests[socket].find("Content-Length: ") + 16, 10).c_str());
			if (_requests[socket].size() >= len + endOfHeader) {
				return EXIT_SUCCESS;
			} else {
				return -1; //content length does not match the body size
			}
		}
		else { //no content length, not chunked encoding, no multipart
			return EXIT_SUCCESS;
		}	
	}
	return -1;
}

void Server::processChunk(int socket) {
	std::string heads = _requests[socket].substr(0, _requests[socket].find("\r\n\r\n"));
	std::string chunks = _requests[socket].substr(_requests[socket].find("\r\n\r\n") + 4, _requests[socket].size() - 1);
	std::string subChunk = chunks.substr(0, 100);
	std::string body = "";
	int chunkSize = strtol(subChunk.c_str(), NULL, 16);
	size_t i = 0;

	while (chunkSize) {
		i = chunks.find("\r\n", i) + 2;
		body += chunks.substr(i, chunkSize);
		i += chunkSize + 2;
		subChunk = chunks.substr(i, 100);
		chunkSize = strtol(subChunk.c_str(), NULL, 16);
	}
	_requests[socket] = heads + "\r\n\r\n" + body;
}

void Server::processRequest(int socket) {

	if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos &&
		_requests[socket].find("Transfer-Encoding: chunked") < _requests[socket].find("\r\n\r\n")) {
		processChunk(socket);
	}

    if (!_requests[socket].empty()) {
        Request request(_requests[socket]);
		if (_ret[socket] != 200) {
			request.setRet(_ret[socket]);
		}
		if (DEBUG == 1) {
			request.printRequestDebug(CYAN); // print DEBUG info
		} else {
			std::cout << request << std::endl;
		}
        HttpData    httpData = setHttpData(request);
        Response    response(httpData, request);
        _responses.insert(std::make_pair(socket, response.getResponse(request)));
    }
	_ret.erase(socket);
	_requestsHeader.erase(socket);
    _requests.erase(socket);
}

int Server::sendResponse(int socket) {
    int ret = send(socket, _responses[socket].c_str(), _responses[socket].size(), 0);
    if (ret == -1) { //send failed
        close(socket);
        _responses.erase(socket);
        return -1;
    }
    else {
        if (static_cast<unsigned long>(ret) >= _responses[socket].size()) {
//            _responses.erase(socket);  //clean is done in Remove Client, testing
            return 0;
        }
        else {
            return 1;
        }
    }
}

HttpData Server::setHttpData(Request &request) {
	HttpData httpData;
    std::string hostName = request.getHost().substr(0, request.getHost().find_last_of(':'));
    std::vector<std::string>::iterator it = std::find(_serverName.begin(), _serverName.end(), hostName);

	// allow localhost and localhost ip (127.0.0.1) when is present in the url as well, e.g. localhost:8085
    if (it != _serverName.end() || hostName == "localhost" || hostName == "127.0.0.1" ) {
        httpData.setServerName(hostName);
        httpData.setPort(_port);
        httpData.setHost(_host);
        httpData.setMaxClientBody(_clientMaxBodySize);
        httpData.setErrorPages(_errorPage);
        httpData.setLocations(_locations);
    } else {
        for (size_t i = 0; i < _relatedServers.size(); i++) {
            std::vector<std::string>    serverNames = _relatedServers[i].getServerName();
            std::vector<std::string>::iterator itName = std::find(serverNames.begin(), serverNames.end(), hostName);
            if (itName != serverNames.end() || hostName == "localhost" || hostName == "127.0.0.1" ) {
                httpData.setServerName(hostName);
                httpData.setPort(_relatedServers[i].getPort());
                httpData.setHost(_relatedServers[i].getHost());
                httpData.setMaxClientBody(_relatedServers[i].getClientMaxBodySize());
                httpData.setErrorPages(_relatedServers[i].getErrorPage());
                httpData.setLocations(*(_relatedServers[i].getLocations()));
                return httpData;
            }
        }
        httpData.setServerName("NF");
    }
	return httpData;
}

bool Server::isSameListen(const Server &other) {
	return _port == other.getPort() && _host == other.getHost();
}

void Server::addRelatedServers(Server server) {
	_relatedServers.push_back(server);
	std::set<std::string>::iterator it = server._serverNameSet.begin();
	for (; it != server._serverNameSet.end(); it++) {
		if (!_serverNameSet.insert(*it).second) {
			throw std::invalid_argument(ERROR " Duplicate server names" RESET);
		}
	}
}

void Server::checkDuplicateLocationPath(const std::string& path) {
	std::vector<Location> *locations = getLocations();
	std::vector<Location>::iterator itLoc = locations->begin();
	for(; itLoc != locations->end(); itLoc++){
		if (itLoc->getPathLocation() == path){
			throw std::logic_error(ERROR " Duplicate location '" + path + "'.");
		}
	}
}
