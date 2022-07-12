//
// Created by Diani on 20/06/2022, last update by Anna 08/07/2022.
//

// TODO: set maken van listen met host en port??
// TODO: uitzoeken hoe het zit met ports / server_names / hosts en andere server eigenschappen
// TODO: comments met beschrijving functie toevoegen aan iedere functie 
// TODO: checken waarom bij browser clients er in 1 call 2 clients aangemaakt worden 
//       (misschien omdat er geen reply wordt gegeven door de server: uitproberen)?
// TODO: timeout toevoegen

#include "settings.hpp"
#include "Webserver.hpp"

// Kijken of hier niet al meer geinitialiseerd moet worden: bijvoorbeeld de FD_ZERO's
Webserver::Webserver(void)
	: 	_maxSocket(0) {
	std::cout << "Webserver created" << std::endl;
}

// TODO: Checken of deep copy van server vector gemaakt moet worden
Webserver::Webserver(const Webserver & src) {
	*this = src;
}

// TODO: Checken of deep copy van server vector gemaakt moet worden
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
	Server     newServer;

	newServer.configServer();
	_servers.push_back(newServer);
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

		// test
		std::cout << _maxSocket << std::endl;
	}
}

// The webserver runs through all the fd's of all the servers to find sockets that are ready to read or write

void    Webserver::runWebserver(void) {
	// initialise fd_sets for ready sockets
	FD_ZERO(&_readyRead);
	FD_ZERO(&_readyWrite);
	int running = 1;

	while (running) {
		std::cout << "+++++++ Waiting for connection ++++++++" << std::endl;
		updateReadySockets();
		// loop through all servers
		for (std::vector<Server>::iterator it = _servers.begin(); it < _servers.end(); it++) {
			// loop through all existing sockets
			for (int i = 0; i < _maxSocket + 1; i++) {
				if (FD_ISSET(i, &_readyRead)) {
					// new connection
					if (i == it->getServerSocket()) {
						int newSocket = it->acceptConnection();
						// new client is added to _current_sockets
						FD_SET(newSocket, &_currentSockets);
						// _max_socket is updated with new socket
						updateMaxSocket(newSocket);
					// existing connection
					} else {
						std::cout << "Handling connection" << std::endl;
						// klopt dit: moet niet de lijst met ready worden aangepast hier?
						FD_CLR(i, &_currentSockets);
					}
				}
			} // loop through all sockets
		} // loop through all servers
	}
}

/*
** Function that updates the fd_sets _ready_read and _ready_write
*/
void	Webserver::updateReadySockets(void) {
	_readyRead = _currentSockets;
	//arguments of select: 1: setsize, 2: reading_sockets, 3: writing_sockets, 4: errors, 5: timeout
	if (select(_maxSocket + 1, &_readyRead, &_readyWrite, NULL, NULL) < 0) {
		throw (std::runtime_error("Select failed"));
	}
}

void Webserver::updateMaxSocket(int newSocket) {
	if (newSocket > _maxSocket)
		_maxSocket = newSocket;
}
