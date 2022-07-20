//
// Created by Diani on 20/06/2022, last update by Anna 08/07/2022.
//

// TODO: A server can listen multiple ports, each port has its own socket and connected clients
//		 Two options: 1. define each socket as a separate server (rest of server information is the same)
//					  2. make it possible to have multiple tcp_sockets for 1 server, each with his own clients
//						 last one is more difficult in looping through sockets and servers
// TODO: Find out what to do with location / location block: what is the job of the server there?
// TODO: Update max_socket fd after removing a client
// TODO: Add a writing part to running function

#include "settings.hpp"
#include "Webserver.hpp"

Webserver::Webserver(void)
	: 	_maxSocket(0),
		_activeClients(0) {
	FD_ZERO(&_currentSockets);
}

Webserver::Webserver(const Webserver & src) {
	if (this != &src) {
		*this = src;
	}
}

Webserver& Webserver::operator=(const Webserver & rhs) {
	if (this != & rhs) {
		_maxSocket = rhs._maxSocket;
		_activeClients = rhs._activeClients;
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
	}
}

// The webserver runs through all the fd's of all the servers to find sockets that are ready to read or write
void    Webserver::runWebserver(void) {
	int 			running = 1;
	int				ret = 0;
	int				ready = 0;
	struct timeval	timeout;
	
	// temp as long as Request class isn't ready
	uint8_t			buff[MAXLINE + 1];
	uint8_t			recvline[MAXLINE + 1];

	std::cout << GREEN "Webserver running" RESET << std::endl;
	while (running) {
		if (ready == 0) {
			if (_activeClients == 0)
				std::cout << CYAN "+++++++ Waiting for connection ++++++++" RESET << std::endl;
			while (ready == 0) {
				timeout.tv_sec  = 1;
				timeout.tv_usec = 0;
				ready = updateReadySockets(timeout);
			}
		}
		else if (ready > 0) {
			for (std::vector<Server>::iterator itServer = _servers.begin(); itServer < _servers.end(); itServer++) {
				// loop through all existing sockets
				for (int i = 0; i <= _maxSocket; i++) {
					// handling the 'ready to read' sockets
					if (FD_ISSET(i, &_readyRead)) {
						// new connection
						if (i == itServer->getServerSocket()) {
							std::cout << "new connection of socket " << i << std::endl; // test: delete later
							int newSocket = itServer->acceptConnection();
							// new client is added to set of _current_sockets
							FD_SET(newSocket, &_currentSockets);
							updateMaxSocket(newSocket);
							_activeClients++;
							// TODO: check if this is the right place
							ready = updateReadySockets(timeout);
						// existing connection
						} else {
							std::vector<Client> clients = itServer->getClients();
							for (std::vector<Client>::iterator itClient = clients.begin(); itClient < clients.end(); itClient++) {
								if (i == itClient->getClientSocket()) {
									std::cout << "existing connection of socket " << i << std::endl; // test: delete later
									// handling connection: could be replaced to class Request (HTTP)
									memset(recvline, 0, MAXLINE);
									// read the clients message
									while ((ret = read(i, recvline, MAXLINE - 1)) > 0) {
										std::cout << recvline << std::endl;
										if (recvline[ret - 1] == '\n') {
											break;
										}
										itClient->setClientTimeStamp();
										memset(recvline, 0, MAXLINE);
									}
									// write an answer to the client
									snprintf((char*)buff, sizeof(buff), "HTTP/1.1 200 OK\r\n\r\n<HTML>Hello</HTML>");
									write(i, (char*)buff, strlen((char*)buff));
									FD_CLR(i, &_currentSockets);
									ready = updateReadySockets(timeout);
								}
							}
						}
					}
				} // loop through all sockets
			} // loop through all servers
		}
	}
}

/*
** Function that updates the fd_sets _readyRead and _readyWrite
*/
int	Webserver::updateReadySockets(struct timeval timeout) {
	int ready;

	FD_ZERO(&_readyRead);
	FD_ZERO(&_readyWrite);
	_readyRead = _currentSockets;
	ready = select(_maxSocket + 1, &_readyRead, &_readyWrite, NULL, &timeout);
	checkTimeout();
	return ready;
}

// ADD DESCRIPTION
void Webserver::updateMaxSocket(int newSocket) {
	if (newSocket > _maxSocket)
		_maxSocket = newSocket;
}

// ADD DESCRIPTION
void Webserver::clear(void) {
	for (std::vector<Server>::iterator itServer = _servers.begin(); itServer < _servers.end(); itServer++) {
		std::cout << itServer->getServerSocket() << " closed" << std::endl; // test: delete later
		close(itServer->getServerSocket());

		_servers.erase(itServer);
	}
}

/*
** Function that checks for every server if there are clients that have a connection but are not responding anymore.
** The function compares the timestamp of the clients connection with the current time. If that's more than the servers 
** time_out the client's socket is closed and the client is removed from the list of clients.
** JOBS:
** 1. Loop through <vector> with all servers
** 2. For each server loop through <vector> with all clients
** 3. Checks for each client if the current time - connection time < 10
** 4. If not it removes the client (and close the socket)
*/
void Webserver::checkTimeout(void) {
	struct		timeval tv;
	long long	seconds;

	for (std::vector<Server>::iterator itServer = _servers.begin(); itServer < _servers.end(); itServer++) {
		std::vector<Client> clients = itServer->getClients();
		for (std::vector<Client>::iterator itClient = clients.begin(); itClient < clients.end(); itClient++) {
			gettimeofday(&tv, NULL);
			seconds = tv.tv_sec - itClient->getClientTimeStamp();
			if (seconds >= itServer->getTimeout()) {
				std::cout << "Client " << itClient->getClientSocket() << " timed out: " << seconds << " seconds" << std::endl; // test: delete later
				FD_CLR(itClient->getClientSocket(), &_currentSockets);
				itServer->removeClient(itClient->getClientSocket());
				// test if this is the right place
				_activeClients--;
			}
		}
	}
}
