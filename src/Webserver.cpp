//
// Created by Diani on 20/06/2022, last update by Anna 08/07/2022.
//

// TODO: Add descriptions to all functions

#include "settings.hpp"
#include "Webserver.hpp"

Webserver::Webserver(void)
	: 	_maxSocket(0),
		_activeClients(0),
		_activeServers(0) {
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
		_activeServers = rhs._activeServers;
		_currentSockets = rhs._currentSockets;
		_servers = rhs._servers;
	}
	return *this;
}

Webserver::~Webserver(void) {
	std::cout << RED "Webserver is destroyed" RESET << std::endl;
	return;
}

/*
** DESCRIPTION
** Function that creates the servers from the configuration file and adds them to the <vector> _servers
** JOBS
** 1. follows...
*/
void    Webserver::loadConfiguration(std::string configFile) {	
	std::cout << configFile << std::endl;	// test: delete later
	// Diana: in this spot you can add a function in which you send the configFile to the Configurator.cpp file
	// and do your stuff. After you finished, we can create servers based on your configuration
	Server     newServer(80);				// test: delete later
	_servers.push_back(newServer);			// test: delete later
	Server     newServer2(81);				// test: delete later
	_servers.push_back(newServer2);			// test: delete later
}

/*
** DESCRIPTION
** Function that loops through all servers to create a connection and put them in listening state
** JOBS
** 1. Initialisation of fd_set with current sockets
** 2. Loop through <vector> with all servers to do their setup (setupServer())
** 3. Updates the _max_socket fd
*/
void    Webserver::createConnection(void) {
	FD_ZERO(&_currentSockets);

	for (std::vector<Server>::iterator it = _servers.begin(); it < _servers.end(); it++) {
		if (it->setupServer() == EXIT_FAILURE) {
			_servers.erase(it--);
		}
		else {
			FD_SET(it->getServerSocket(), &_currentSockets);
			updateMaxSocket(it->getServerSocket(), ADD);
			_activeServers++;
		}
	}
	if (_activeServers < 1)
		throw (std::logic_error("There are no active servers"));
}

// ADD DESCRIPTION
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
							//std::cout << "new connection of socket " << i << std::endl; // test: delete later
							int newSocket = itServer->acceptConnection();
							// new client is added to set of _current_sockets
							FD_SET(newSocket, &_currentSockets);
							updateMaxSocket(newSocket, ADD);
							_activeClients++;
							// TODO: check if this is the right place
							ready = updateReadySockets(timeout);
						// existing connection
						} else {
							std::vector<Client> clients = itServer->getClients();
							for (std::vector<Client>::iterator itClient = clients.begin(); itClient < clients.end(); itClient++) {
								if (i == itClient->getClientSocket()) {
									//std::cout << "existing connection of socket " << i << std::endl; // test: delete later
									// handling connection: could be replaced to class Request
									memset(recvline, 0, MAXLINE);
									// read the clients message
									while ((ret = read(i, recvline, MAXLINE - 1)) > 0) {
										std::cout << recvline << std::endl;
										if (recvline[ret - 1] == '\n') {
											break;
										}
										// renew the timestamp of the client in order to track it's timeout
										itClient->setClientTimeStamp();
										memset(recvline, 0, MAXLINE);
									}
									// write an answer to the client: could be replaced to class Response
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
** DESCRIPTION
** Function that updates the fd_sets _readyRead and _readyWrite
*/
int	Webserver::updateReadySockets(struct timeval timeout) {
	int ready;

	FD_ZERO(&_readyRead);
	FD_ZERO(&_readyWrite);
	_readyRead = _currentSockets;
	ready = select(_maxSocket + 1, &_readyRead, &_readyWrite, NULL, &timeout);
	// FOUTMELDING VOOR SELECT TOEVOEGEN??
	checkTimeout();
	return ready;
}

// ADD DESCRIPTION
void Webserver::updateMaxSocket(int socket, int type) {
	if (type == ADD && socket > _maxSocket) {
		_maxSocket = socket;
	}
	else if (type == REMOVE && socket == _maxSocket) {
		int tempMaxSocket = 0;
		for (std::vector<Server>::iterator itServer = _servers.begin(); itServer < _servers.end(); itServer++) {
			if (itServer->getServerSocket() > tempMaxSocket)
				tempMaxSocket = itServer->getServerSocket();
			std::vector<Client> clients = itServer->getClients();
			for (std::vector<Client>::iterator itClient = clients.begin(); itClient < clients.end(); itClient++) {
				if (itClient->getClientSocket() > tempMaxSocket)
					tempMaxSocket = itClient->getClientSocket();
			}
		}
		_maxSocket = tempMaxSocket;	
	}
	std::cout << "maxSocket = " << _maxSocket << std::endl; // test:delete later
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
** DESCRIPTION
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
				std::cout << RED "Client " << itClient->getClientSocket() << " timed out: " << seconds << " seconds" RESET << std::endl; // test: delete later
				FD_CLR(itClient->getClientSocket(), &_currentSockets);	
				itServer->removeClient(itClient->getClientSocket());
				updateMaxSocket(itClient->getClientSocket(), REMOVE);
				_activeClients--;
			}
		}
	}
}
