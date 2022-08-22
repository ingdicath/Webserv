//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#pragma once

#include <vector>
#include <string>
#include <set>
#include <map>

class Location {
public:
	Location();
	explicit Location(std::string path);
	Location(Location const &obj);
	Location &operator=(Location const &obj);
	virtual ~Location();

	//Set functions
	void setPathLocation(const std::string &pathLocation);

private:
	std::string _pathLocation;
	std::string _upload;
	std::string _root;
	bool _autoindex;
	std::set<std::string> _accepted_methods;
	std::map<int, std::string> _redirection;
	std::pair<int, std::string> _cgi;
};

