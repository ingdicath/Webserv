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

	explicit Location(bool autoindex);

	Location(Location *pLocation);

	virtual ~Location();

private:
	std::string _locationPath;
	std::string _root;
	std::set<std::string> _accepted_methods;
	std::vector<std::string> _index;
	bool _autoindex;
	std::map<std::string, std::string> _cgi; // in case we decide accept other formats than .py, could be php?
	std::string _upload;
	std::map<int, std::string> _redirection;
};

