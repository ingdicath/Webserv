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

	void setPathLocation(const std::string &pathLocation);

private:
	std::string _pathLocation;
	std::set<std::string> _accepted_methods;
	std::map<int, std::string> _return;
	std::string _root;
	bool _autoindex;
	std::pair<int, std::string> _cgi;
	std::string _upload;
};

