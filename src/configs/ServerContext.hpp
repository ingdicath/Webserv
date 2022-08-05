//
// Created by Diani on 04/08/2022.
//
#pragma once

#include <string>
#include <vector>
#include <map>
#include "LocationContext.hpp"

class ServerContext {
public:
	ServerContext();

	virtual ~ServerContext();

	//TODO: Make it private
	bool _isValidPortRange(const std::string &port);
	bool _isValidIpv4Address(const std::string &ipAddress);
	bool _isValidServerName(const std::string &serverName);

	class InvalidPortRangeException : public std::exception {
	public:
		const char *what() const throw(){
			return "Config error: invalid port value: '";
		}
	};



private:
	int _port;
	std::string _host;
	std::vector<std::string> _serverName;
	std::string _index;
	std::map<int, std::string> _errorPages;
	long _clientMaxBodySize;
	std::vector<LocationContext> _locations;
};
