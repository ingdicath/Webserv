//
// Created by Diani on 20/06/2022, last update by Anna 08/07/2022.
//

// TODO: A server can listen multiple ports, each port has its own socket and connected clients
//		 Two options: 1. define each socket as a separate server (rest of server information is the same)
//					  2. make it possible to have multiple tcp_sockets for 1 server, each with his own clients
//						 last one is more difficult in looping through sockets and servers
// TODO: Write descriptions with every function
// TODO: Build-in time-out functionality to select function
// TODO: Decide whether to use send en receive instead of read en write
// TODO: Find out if the client needs to be removed after the connection closes

#include "settings.hpp"
#include "Webserver.hpp"

Webserver::Webserver(void)
	: 	_maxSocket(0) {
	FD_ZERO(&_currentSockets);
}

// TODO: Check if deep copy of webserver is necessary
Webserver::Webserver(const Webserver & src) {
	*this = src;
}

// TODO: Check if deep copy of webserver is necessary
Webserver& Webserver::operator=(const Webserver & rhs) {
	if (this != & rhs) {
		_maxSocket = rhs._maxSocket;
		_currentSockets = rhs._currentSockets;
		_servers = rhs._servers;
	}
	return *this;
}

Webserver::~Webserver(void) {
	std::cout << "Webserver destroyed" << std::endl;
	return;
}

/*
** Function that creates the servers from the configuration file and add them to the <vector> _servers
*/
void    Webserver::loadConfiguration(void) {
	Server     newServer(85);
	_servers.push_back(newServer);
	Server     newServer2(80);
	_servers.push_back(newServer2);
}

/*
** Function that loops through all servers to create a connection and put them in listening state
** JOBS
** 1. Initialisation of fd_set with current sockets
** 2. Loop through <vector> with all servers to do their setup (setupServer())
** 3. Updates the _max_socket fd
*/
void    Webserver::createConnection(void) {
	FD_ZERO(&_currentSockets);

	for (std::vector<Server>::iterator it = _servers.begin(); it < _servers.end(); it++) {
		it->setupServer();
		FD_SET(it->getServerSocket(), &_currentSockets);
		updateMaxSocket(it->getServerSocket());

		std::cout << _maxSocket << std::endl; // test: delete later
	}
}

// The webserver runs through all the fd's of all the servers to find sockets that are ready to read or write

void    Webserver::runWebserver(void) {
	int 			running = 1;
	int				ret = 0;
	
	// temp as long as Request class isn't ready
	uint8_t			buff[MAXLINE + 1];
	uint8_t			recvline[MAXLINE + 1];

	std::cout << "+++++++ Waiting for connection ++++++++" << std::endl;
	while (running) {
		ret = updateReadySockets();
		std::cout << ret << std::endl;
		// loop through all servers
		for (std::vector<Server>::iterator it = _servers.begin(); it < _servers.end(); it++) {
			// loop through all existing sockets
			for (int i = 0; i < _maxSocket + 1; i++) {
				if (FD_ISSET(i, &_readyRead)) {
					// new connection
					if (i == it->getServerSocket()) {
						std::cout << "new connection of socket" << i << std::endl;
						int newSocket = it->acceptConnection();
						// new client is added to _current_sockets
						FD_SET(newSocket, &_currentSockets);
						// _max_socket is updated with new socket
						updateMaxSocket(newSocket);
					// existing connection
					} else {
						std::vector<Client> clients = it->getClients();
						for (std::vector<Client>::iterator ite = clients.begin(); ite < clients.end(); ite++) {
							if (i == ite->getClientSocket()) { 
								std::cout << "existing connection of socket " << i << std::endl; // test: delete later
								// handling connection: could be replaced to class Request (HTTP)
								memset(recvline, 0, MAXLINE);
								// read the clients message
								while ((ret = read(i, recvline, MAXLINE-1)) > 0) {
									std::cout << recvline << std::endl;
									if (recvline[ret - 1] == '\n') {
										std::cout << "break" << std::endl;
										break;
									}
									memset(recvline, 0, MAXLINE);
								}
								// write an answer to the client
								snprintf((char*)buff, sizeof(buff), "HTTP/1.1 200 OK\r\n\r\nHello");
								write(i, (char*)buff, strlen((char*)buff));
								FD_CLR(i, &_currentSockets);
								close(i);
							}
						}
					}
				}
			} // loop through all sockets
		} // loop through all servers
	}
}

/*
** Function that updates the fd_sets _readyRead and _readyWrite
*/
int	Webserver::updateReadySockets(void) {
	int ret;

	FD_ZERO(&_readyRead);
	FD_ZERO(&_readyWrite);
	_readyRead = _currentSockets;
	//arguments of select: 1: setsize, 2: reading_sockets, 3: writing_sockets, 4: errors, 5: timeout
	ret = select(_maxSocket + 1, &_readyRead, &_readyWrite, NULL, NULL);
	return ret;
}

void Webserver::updateMaxSocket(int newSocket) {
	if (newSocket > _maxSocket)
		_maxSocket = newSocket;
}
