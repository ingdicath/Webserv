/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Webserver.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 08:34:22 by annaheister   #+#    #+#                 */
/*   Updated: 2022/10/25 08:34:22 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <algorithm>
#include <ostream>
#include "Server.hpp"

enum e_action
{
	ADD,
	REMOVE,
	WRITE,
	READ
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
	int		findReadySockets(struct timeval timeout);
	void	updateSockets(int socket, int type, int subtype);
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
