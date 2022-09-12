//
// Created by Diana catherine Salamanca leguizamon on 5/24/22.
//

#pragma once

#include "Location.hpp"
#include "Client.hpp"
#include "../utils/settings.hpp"
#include "../http/Request.hpp"

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <vector>
#include <map>
#include <set>
#include <string>

#include <iostream>
#include <stdexcept>	// used for exceptions
#include <unistd.h>		// used for read/write/close

class Server {
public:
	Server(void);
	Server(int port);
	Server(Server const & src);
	Server& operator=(Server const & rhs);
	virtual ~Server(void);

	//Socket functions
	int		setupServer(void);
	int		acceptConnection(void);
	void	addClient(int newSocket, struct	sockaddr_in clientAddr);
	void	removeClient(int thisSocket);

	class setupException : public std::exception {
	public:
		virtual const char *what() const throw();
	};


	//Get functions
	int								getPort(void) const;
	const std::string				&getHost() const;
	std::vector<Location>			*getLocations();
	const std::vector<std::string>	&getServerName() const;
	const std::string				&getIndex() const;
	long							getClientMaxBodySize() const;
	const							std::map<int, std::string> &getErrorPage() const;

	int								getServerSocket(void) const;
	std::vector<Client>				getClients(void) const;
	uint32_t						getTimeout(void) const;


	//Set functions
	void					setPort(int port);
	void					setHost(std::string host);
	void					setServerName(const std::vector<std::string> &serverName);
	void					setClientMaxBodySize(long clientMaxBodySize);

	void					addErrorPage(const std::pair<int, std::string> &errorPage);
	void					addLocation(Location location);

    int     recvRequest(int clientId);
    void    processRequest(int clientId);
    void    processChunk(int clientId);

private:
// Configuration file
	int							_port;
	std::string					_host;
	std::vector<std::string>	_serverName;
	std::string					_index;
	std::map<int, std::string>	_errorPage;					// Explore how this works
	long						_clientMaxBodySize;
	std::vector<Location>		_locations;				// Explore how this works

// Variables that help to validate duplicate values
	bool						_flagPort; // option #1 for validate duplicate directives
	bool						_flagHost;

	int							_timeOut;

// Connection
	int							_serverSocket;
	struct sockaddr_in			_serverAddr;
	std::vector<Client>			_clients;

    std::map<int, std::string>  _requests;

	void	_setDefaultErrorPages();
};
