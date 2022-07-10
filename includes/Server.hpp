//
// Created by Diana catherine Salamanca leguizamon on 5/24/22.
//

#ifndef WEBSERV_SERVER_H
#define WEBSERV_SERVER_H

#pragma once

// #include "Location.hpp"
#include "Client.hpp"

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <fcntl.h>

#include <vector>
#include <string>
// #include <map>
// #include <set>

#include <iostream>
#include <stdexcept>

#include <unistd.h> // used for read/write/close

class Server {
public:
	Server(void);
	Server(Server const & src);
	Server& operator=(Server const & rhs);
	virtual ~Server(void);

	//Configuration functions
	void	configServer(void);

	//Socket functions
	void	setupServer(void);
	int		acceptConnection(void);

	//Get functions
	int		getServerSocket(void);
	int		getPort(void);
	
private:
	//Configuration
	int					_port;
	//std::string 		_host;										// Format??
    std::string			_server_name;
    int					_error_page;								// Explore how this works
	//int					_time_out								// Explore how this works
	//std::map<std::string, std::string>	_cgi_file_types;		// Associative container that store elements formed by a combination of a key value and a mapped value
	//std::vector<Location>		_location_block;					// Explore how this works
	std::size_t			_client_max_body_size;						// WHAT IS THIS?

	//Connection
	int					_server_socket;
	struct sockaddr_in 	_server_addr;
	std::vector<Client> _clients;

};

#endif //WEBSERV_SERVER_H

// class Server {
// public:
// 	Server(void);
// 	Server(Server *pServer);
// 	Server(Server const & src);
// 	Server &operator=(Server const & rhs);
// 	virtual ~Server(void);
	// void validateAndSetListen(std::vector<std::string> values);

	// const std::vector<std::string>				&getServerName() const;
	// const std::vector<Location>					&getLocations() const;
	// const std::set<std::string>					&getListen() const;
	// const std::map<std::size_t, std::string>	&getErrorPage() const;
	// size_t 										getClientMaxBodySize() const;


	/** setters, check which of this we need to use*/

//	void setListen(const std::string &str);
//	void setListenFromInput(const std::string& str);
	// void setPort(const std::string &str);
	// void setLocations(const std::vector<Location> &locations);
	// void setListen1(const std::set<std::string> &listen);
	// void setServerName(const std::vector<std::string> &serverName);
	// void setErrorPage(const std::map<std::size_t, std::string> &errorPage);
	// void setClientMaxBodySize(size_t clientMaxBodySize);

	//TODO make private later
//	std::vector<Location> _locations;

//private:
	/** server **/
	// std::set<std::string> 				_listen;
	// std::vector<std::string> 			_server_name;
	// std::map<std::size_t, std::string>	_error_page;
	// std::size_t 						_client_max_body_size;
//};
