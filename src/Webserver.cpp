//
// Created by Diani on 20/06/2022, last update by Anna 08/07/2022.
//

// TODO: error procedure aanpassen
// TODO: set maken van listen met host en port?? 
// TODO: kijken waar maxclientbodysize moet worden opgeslagen
// TODO: makefile werkend krijgen
// TODO: comments met beschrijving functie toevoegen aan iedere functie 
// TODO: checken waarom bij browser clients er in 1 call 2 clients aangemaakt worden
// TODO: overal kijken waar er een const gebruikt moet worden
// TODO: timeout toevoegen

#include "../includes/settings.hpp"
#include "../includes/Webserver.hpp"

// Kijken of hier niet al meer geinitialiseerd moet worden: bijvoorbeeld de FD_ZERO's
Webserver::Webserver(void)
	: 	_max_socket(0) {
	std::cout << "Webserver created" << std::endl;
}

Webserver::Webserver(const Webserver & src) {
	*this = src;
}

Webserver& Webserver::operator=(const Webserver & rhs) {
	if (this != & rhs) {
		_max_socket = rhs._max_socket;
		_current_sockets = rhs._current_sockets;
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
	Server     new_server;

	new_server.configServer();
	_servers.push_back(new_server);
}

/*
** Function that loops through all servers to create a connection and put them in listening state
** JOBS
** 1. Initialisation of fd_set with current sockets
** 2. Loop through <vector> with all servers to do their setup (setupServer())
** 3. Updates the _max_socket fd
*/
void    Webserver::createConnection(void) {
	FD_ZERO(&_current_sockets);

	for (std::vector<Server>::iterator it = _servers.begin(); it < _servers.end(); it++) {
		it->setupServer();
		FD_SET(it->getServerSocket(), &_current_sockets);
		updateMaxSocket(it->getServerSocket());

		// test
		std::cout << _max_socket << std::endl;
	}
}

// The webserver runs through all the fd's of all the servers to find sockets that are ready to read or write

void    Webserver::runWebserver(void) {
	// initialise fd_sets for ready sockets
	FD_ZERO(&_ready_read);
	FD_ZERO(&_ready_write);

	while (true) {
		std::cout << "+++++++ Waiting for connection ++++++++" << std::endl;
		updateReadySockets();
		// loop through all servers
		for (std::vector<Server>::iterator it = _servers.begin(); it < _servers.end(); it++) {
			// loop through all existing sockets
			for (int i = 0; i < _max_socket + 1; i++) {
				if (FD_ISSET(i, &_ready_read)) {
					// new connection
					if (i == it->getServerSocket()) {
						int new_socket = it->acceptConnection();
						// new client is added to _current_sockets
						FD_SET(new_socket, &_current_sockets);
						// _max_socket is updated with new socket
						updateMaxSocket(new_socket);
					// existing connection
					} else {
						std::cout << "Handling connection" << std::endl;
						// klopt dit: moet niet de lijst met ready worden aangepast hier?
						FD_CLR(i, &_current_sockets);
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
	_ready_read = _current_sockets;
	//arguments of select: 1: setsize, 2: reading_sockets, 3: writing_sockets, 4: errors, 5: timeout
	if (select(_max_socket + 1, &_ready_read, &_ready_write, NULL, NULL) < 0) {
		throw (std::runtime_error("Select failed"));
	}
}

void Webserver::updateMaxSocket(int new_socket) {
	if (new_socket > _max_socket)
		_max_socket = new_socket;
}
