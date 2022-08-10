//
// Created by Diana catherine Salamanca leguizamon on 5/24/22.
//

#pragma once

#include "Location.hpp"
#include "Client.hpp"
#include "../configs/Configurator.hpp"
#include "../utils/settings.hpp"

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

	//Configuration functions
	void	configServer(void);

//	// Move to configurator later: functions to validate input from config file
//	void validateAndSetListen(std::vector<std::string> values);
//	void validateAndSetServerNames(std::vector<std::string> values);

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
	int						getPort(void) const;
	const std::string		&getHost() const;
	std::vector<Location>	getLocations();

	//Set functions
	void setPort(int port);


	int						getServerSocket(void) const;
	std::vector<Client>		getClients(void) const;
	long long				getTimeout(void) const;


private:
// Configuration
	int							_port;
	std::string					_host;
	std::vector<std::string>	_serverName;
	std::string					_index;
	std::map<int, std::string>	_errorPage;					// Explore how this works
//	long						_clientMaxBodySize;
	std::vector<Location>		_locations;				// Explore how this works



	int										_timeOut;
	//Connection
	int						_serverSocket;
	struct sockaddr_in 		_serverAddr;
	std::vector<Client> 	_clients;

};
