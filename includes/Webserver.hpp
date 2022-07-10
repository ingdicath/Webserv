//
// Created by Diani on 20/06/2022.
//

#ifndef WEBSERV_WEBSERVER_H
#define WEBSERV_WEBSERVER_H

#pragma once

#include "Server.hpp"
#include <vector>

class Webserver {
public:
	Webserver(void);
	Webserver(Webserver const & src);
	Webserver& operator=(Webserver const & rhs);
	virtual ~Webserver(void);

	void	loadConfiguration(void);
	void	createConnection(void);
	void	runWebserver(void);
	void	updateReadySockets(void);
	void	updateMaxSocket(int new_socket);

private:
	int						_max_socket;
	fd_set					_current_sockets;
	fd_set					_ready_read;
	fd_set					_ready_write;
	std::vector<Server>		_servers;
};

#endif //WEBSERV_WEBSERVER_H
