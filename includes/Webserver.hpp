//
// Created by Diani on 20/06/2022.
//

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
	int		updateReadySockets(struct timeval timeout);
	void	updateMaxSocket(int new_socket);
	void	checkTimeout(void);
	void	clear(void);

private:
	int						_maxSocket;
	fd_set					_currentSockets;
	fd_set					_readyRead;
	fd_set					_readyWrite;
	std::vector<Server>		_servers;

};

