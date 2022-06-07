//
// Created by Diana catherine Salamanca leguizamon on 5/24/22.
//

#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "Location.hpp"

class Server {
public:
	Server();

	virtual ~Server();

	Server(Server const &obj);

	Server &operator=(Server const &obj);

	void setListen(const std::string& str);
//	void setListenFromInput(const std::string& str);
	void setPort(const std::string& str);





private:
public:
	const std::vector<std::string> &getServerName() const;

private:

	/** server **/
	std::set<std::string> _listen;
	std::vector<std::string> _server_name;
	std::map<std::size_t, std::string> _error_page;
	std::size_t _client_max_body_size;
	std::vector<Location> _locations;



};
