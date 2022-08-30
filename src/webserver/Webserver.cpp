//
// Created by Diani on 20/06/2022, last update by Anna 08/07/2022.
//

// TODO: Keep aware of all 'new' objects added to servers or locations. These should be deleted in order to avoid memory leaks

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "../utils/settings.hpp"
#include "Webserver.hpp"
#include "../http/Request.hpp"
#include "../configs/Parser.hpp"

Webserver::Webserver(void)
	: 	_maxSocket(0),
		_activeClients(0),
		_activeServers(0) {
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
	FD_ZERO(&_readyRead);
	FD_ZERO(&_readyWrite);
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
		_servers = rhs._servers;
	}
	return *this;
}

Webserver::~Webserver(void) {
	if (_servers.empty()) {
		std::cout << RED "Unable to create the webserver due to "
					 "configuration file parsing failure." RESET << std::endl;
	} else {
		std::cout << RED "Webserver is destroyed" RESET << std::endl;
	}
}

/*
** DESCRIPTION
** Function that creates the servers from the configuration file and adds them to the <vector> _servers
** JOBS
** 1. follows...
*/
void	Webserver::loadConfiguration(const std::string& configFile) {
	Parser parser;
	_servers = parser.validateConfiguration(configFile);
}




//TODO: Check ports, is not working setting up just one port (15Aug)

/*
** DESCRIPTION
** Function that loops through all servers to create a connection and put them in listening state
** JOBS
** 1. Initialisation of fd_set with current sockets
** 2. Loop through <vector> with all servers to do their setup (setupServer())
** 3. Updates the _max_socket fd
*/
void    Webserver::createConnection(void) {
	for (std::vector<Server>::iterator it = _servers.begin(); it < _servers.end(); it++) {
		if (it->setupServer() == EXIT_FAILURE) {
			_servers.erase(it--);
		}
		else {
			updateSockets(it->getServerSocket(), ADD, 0);
			//FD_SET(it->getServerSocket(), &_readSet);
			//updateMaxSocket(it->getServerSocket(), ADD);
			_activeServers++;
		}
	}
	if (_activeServers < 1)
		throw (std::logic_error("There are no active servers"));
}

void	 Webserver::writeResponse(int clientFD) {
    char	buff[MAXLINE + 1];

    snprintf((char*)buff, sizeof(buff), "HTTP/1.1 200 OK\r\n\r\n<HTML>Hello</HTML>");
    write(clientFD, (char*)buff, strlen((char*)buff));
}

/*
** Parse the HTTP request
** Handle the request (to do)
** Send the response HTTP back to client (to do)
*/
int	 Webserver::takeRequest(std::vector<Client>::iterator itClient, std::vector<Server>::iterator itServer) {
	char	recvline[MAXLINE + 1];
    // this is a static placeholder, change to a variable when the config parsing is done
    long    maxClientBody = 2147483647;
    int     bytesRead;

    Request request(maxClientBody);

    memset(recvline, 0, MAXLINE);
    //read the clients message, until the connection closed
    //problem with this, cannot recieved more than one, don't know why

    do {
        bytesRead = recv(itClient->getClientSocket(), recvline, MAXLINE - 1, 0);
        if (bytesRead > 0) {
            std::cout << "Has read: " << bytesRead << std::endl; // for testing, delete later
            itClient->setClientTimeStamp();
            request.parseRequest(recvline, bytesRead);
            memset(recvline, 0, MAXLINE);
        }
        else if (bytesRead < 0) {
            std::cout << "recv failed" << std::endl; //need more detailed error message
			return EXIT_FAILURE;
        }
    } while (request.isComplete() == false);

	// print out info in the object for testing, delete later
	std::cout << request << std::endl;
	return EXIT_SUCCESS;
}

/*
** DESCRIPTION
** Function that in a loop checks the select function to find sockets that are ready to read or write. When there is one
** (or more) ready sockets then the function runs through all the servers and their clients to handle the requests.
** JOBS
** 1. After starting the 'running' loop a timeout struct is filled is send with the updateReadySockets function
** 2. Within the updateReadySockets function the select function is called to receive the ready sockets.
** 3. If there are ready sockets (ready > 0) the function loops through every server and every client to find matching sockets
** 4. If a ready socket is the server socket, a new connection is made (and a new client added to the server)
** 5. If the ready socket is an existing client, the client's request is handled and a response is sent back (LATER MOVE TO SEPERATE OBJECT)
*/
void    Webserver::runWebserver(void) {
	int 			running = 1;
	int				ready = 0;
	struct timeval	timeout;

	timeout.tv_sec  = 1;
	timeout.tv_usec = 0;
	std::cout << GREEN "Webserver running" RESET << std::endl;
	while (running) {
		if (ready == 0) {
			if (_activeClients == 0)
				std::cout << CYAN "+++++++ Waiting for connection ++++++++" RESET << std::endl;
			while (ready == 0)
				ready = findReadySockets(timeout);
		}
		else if (ready > 0) {
			// loop through all existing servers
			for (std::vector<Server>::iterator itServer = _servers.begin(); itServer < _servers.end(); itServer++) {
				// loop through all existing sockets
				for (int i = 0; i <= _maxSocket; i++) {
					// handling the 'ready to read' sockets
					if (FD_ISSET(i, &_readyRead) && ready > 0) {
						std::cout << i << " = readyRead" << ", ready = " << ready << std::endl;
						// when the server socket is put in the 'ready' set there is a new connection
						if (i == itServer->getServerSocket()) {
							int newSocket = itServer->acceptConnection();
							updateSockets(newSocket, ADD, READ);
							ready--;
						// else it is an existing connection
						} else {
							std::vector<Client> clients = itServer->getClients();
							for (std::vector<Client>::iterator itClient = clients.begin(); itClient < clients.end(); itClient++) {
								if (i == itClient->getClientSocket()) {
									std::cout << "existing connection of socket " << i << std::endl; // test: delete later
									if	(takeRequest(itClient, itServer) == 0)
										updateSockets(i, ADD, WRITE);
									else {
										itServer->removeClient(i);
										updateSockets(i, REMOVE, READ);
									}
									ready--;
								}
							}
						}
					}
					// handling the 'ready to write' sockets
					else if (FD_ISSET(i, &_readyWrite) && ready > 0) {
						std::vector<Client> clients = itServer->getClients();
						for (std::vector<Client>::iterator itClient = clients.begin(); itClient < clients.end(); itClient++) {
							if (i == itClient->getClientSocket()) {
								writeResponse(i);
								itServer->removeClient(i);
								updateSockets(i, REMOVE, WRITE);
								ready--;
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
** JOBS
** 2. Copy the _readSet set into the _readyRead set (because on return select destroys the original set)
** 3. Copy the _writeSet set into the _readyWrite set (because on return select destroys the original set)
** 4. Retrieve the ready sockets (that are ready to read or write) by running the select function with both sets.
** 5. Run the checkTimeout function to see if any of the clients is not responding
*/
int	Webserver::findReadySockets(struct timeval timeout) {
	int ready;

	_readyRead = _readSet;
	_readyWrite = _writeSet;
	ready = select(_maxSocket + 1, &_readyRead, &_readyWrite, NULL, &timeout);
	checkTimeout();
	return ready;
}

/*
** DESCRIPTION - TODO: UPDATEN!!
** Function that updates the highest (max) used file descriptor (socket) in the program to be used in the select function
** to check the 'readyness' of all file descriptors (sockets) until the given max has reached
** JOBS
** 1. When a new socket is opened in the programm (type = ADD) the value of the new socket is added to the vector of allSockets
** 2. When a socket is removed (type = REMOVED), the function finds the socket in the vector, erases it and the max value is retrieved
**
*/
void Webserver::updateSockets(int socket, int type, int subtype) {
	if (type == ADD) {
		if (!subtype) {
			_allSockets.push_back(socket);
			FD_SET(socket, &_readSet);
		}
		if (subtype == READ) {
			_allSockets.push_back(socket);
			FD_SET(socket, &_readSet);
			_activeClients++;
		}
		if (subtype == WRITE) {
			FD_CLR(socket, &_readSet);
			FD_SET(socket, &_writeSet);
		}
	}
	else if (type == REMOVE) {
		if (subtype == READ)
			FD_CLR(socket, &_readSet);
		if (subtype == WRITE)
			FD_CLR(socket, &_writeSet);
		_activeClients--;
		std::vector<int>::iterator it = std::find(_allSockets.begin(), _allSockets.end(), socket);
		_allSockets.erase(it);
	}
	_maxSocket = *std::max_element(_allSockets.begin() + 1, _allSockets.end());
}

/*
** DESCRIPTION
** Function that in a loop closes each server socket and removes the server. Function in called when a blocking error occurs
*/
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
	int			clientSocket;		

	for (std::vector<Server>::iterator itServer = _servers.begin(); itServer < _servers.end(); itServer++) {
		std::vector<Client> clients = itServer->getClients();
		for (std::vector<Client>::iterator itClient = clients.begin(); itClient < clients.end(); itClient++) {
			gettimeofday(&tv, NULL);
			seconds = tv.tv_sec - itClient->getClientTimeStamp();
			if (seconds >= itServer->getTimeout()) {
				clientSocket = itClient->getClientSocket();
				std::cout << RED "Client " << clientSocket << " timed out: " << seconds << " seconds" RESET << std::endl;
				itServer->removeClient(clientSocket);
				updateSockets(clientSocket, REMOVE, READ);
			}
		}
	}
}
