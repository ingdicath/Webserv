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

	Server(Server *pServer);

	virtual ~Server();

	Server(Server const &obj);

	Server &operator=(Server const &obj);

	void validateAndSetListen(std::vector<std::string> values);

	const std::vector<std::string> &getServerName() const;

	const std::vector<Location> &getLocations() const;

	const std::set<std::string> &getListen() const;

	const std::map<std::size_t, std::string> &getErrorPage() const;

	size_t getClientMaxBodySize() const;


	/** setters, check which of this we need to use*/

	void setListen(const std::string &str);

//	void setListenFromInput(const std::string& str);
	void setPort(const std::string &str);

	void setLocations(const std::vector<Location> &locations);

	void setListen1(const std::set<std::string> &listen);

	void setServerName(const std::vector<std::string> &serverName);

	void setErrorPage(const std::map<std::size_t, std::string> &errorPage);

	void setClientMaxBodySize(size_t clientMaxBodySize);

	//TODO make private later
	std::vector<Location> _locations;

private:
	/** server **/
	std::set<std::string> _listen;
	std::vector<std::string> _server_name;
	std::map<std::size_t, std::string> _error_page;
	std::size_t _client_max_body_size;
};
