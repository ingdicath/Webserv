//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include "Location.hpp"

Location::~Location() {}

Location::Location() {}

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
		_return = obj._return;
		_root = obj._root;
		_autoindex = obj._autoindex;
		_cgi = obj._cgi;
		_upload = obj._upload;
	}
	return *this;
}
