//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include "Location.hpp"

#include "../utils/settings.hpp"


Location::Location() : _pathLocation("/"), _root(DEFAULT_ROOT), _autoindex(false) {
	_accepted_methods.insert("GET");
	_accepted_methods.insert("POST");
	_accepted_methods.insert("DELETE");
	_redirection.insert(std::pair<int, std::string>(DEFAULT_REDIRECTION_NUM,
													DEFAULT_REDIRECTION_PAGE));
}

Location::Location(std::string path) : _pathLocation(path), _root(DEFAULT_ROOT),
									   _autoindex(false) {
	_accepted_methods.insert("GET");
	_accepted_methods.insert("POST");
	_accepted_methods.insert("DELETE");
	_redirection.insert(std::pair<int, std::string>(DEFAULT_REDIRECTION_NUM,
													DEFAULT_REDIRECTION_PAGE));
}

Location::~Location() {}

Location::Location(const Location &obj) {
	*this = obj;
}

void Location::setPathLocation(const std::string &pathLocation) {
	_pathLocation = pathLocation;
}

Location &Location::operator=(const Location &obj) {
	if (this != &obj) {
		_pathLocation = obj._pathLocation;
		_accepted_methods = obj._accepted_methods;
		_redirection = obj._redirection;
		_root = obj._root;
		_autoindex = obj._autoindex;
		_cgi = obj._cgi;
		_upload = obj._upload;
	}
	return *this;
}


