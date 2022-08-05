//
// Created by Diani on 04/08/2022.
//

#pragma once
#include <vector>
#include <string>
#include <map>

class LocationContext {
public:
	LocationContext();
	virtual ~LocationContext();

private:
	std::vector<std::string> _accepted_methods;
	std::map<int, std::string> _return;
	std::string _root;
	std::string _autoindex;
	std::map<int, std::string> _cgi;
	std::string _upload;
};
