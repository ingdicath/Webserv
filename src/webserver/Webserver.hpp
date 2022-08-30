//
// Created by Diani on 20/06/2022.
//

#pragma once

#include "Server.hpp"
#include <vector>
#include <algorithm>

enum e_typeMax
{
	ADD,
	REMOVE,
};

class Webserver {
public:
	Webserver(void);
	Webserver(Webserver const & src);
	Webserver& operator=(Webserver const & rhs);
	virtual ~Webserver(void);

	void	loadConfiguration(const std::string& configFile);
	void	createConnection(void);
	void	runWebserver(void);
	int		updateReadySockets(struct timeval timeout);
	void	updateMaxSocket(int socket, int type);
	int	 	takeRequest(std::vector<Client>::iterator itClient, std::vector<Server>::iterator itServer);
	void	writeResponse(int clientFD);
	void	checkTimeout(void);
	void	clear(void);

private:
	int						_maxSocket;
	std::vector<int>		_allSockets;
	int						_activeClients;
	int						_activeServers;
	fd_set					_writeSet;
	fd_set					_readSet;
	fd_set					_readyRead;
	fd_set					_readyWrite;
	std::vector<Server>		_servers;
};

