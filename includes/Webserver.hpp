//
// Created by Diani on 20/06/2022.
//

#pragma once

#include "Server.hpp"
#include <vector>

typedef enum e_typeMax
{
	ADD,
	REMOVE,
}	t_typeMax;

class Webserver {
public:
	Webserver(void);
	Webserver(Webserver const & src);
	Webserver& operator=(Webserver const & rhs);
	virtual ~Webserver(void);

	void	loadConfiguration(std::string configFile);
	void	createConnection(void);
	void	runWebserver(void);
	int		updateReadySockets(struct timeval timeout);
	void	updateMaxSocket(int socket, int type);
	void	checkTimeout(void);
	void	clear(void);

private:
	int						_maxSocket;
	int						_activeClients;
	int						_activeServers;
	fd_set					_currentSockets;
	fd_set					_readyRead;
	fd_set					_readyWrite;
	std::vector<Server>		_servers;
};

