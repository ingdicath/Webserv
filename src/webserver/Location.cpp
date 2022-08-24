//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include "Location.hpp"

#include "../utils/settings.hpp"


Location::Location() : _pathLocation("/"), _root(DEFAULT_ROOT), _autoindex(false) {
	_acceptedMethods.insert("GET");
	_acceptedMethods.insert("POST");
	_acceptedMethods.insert("DELETE");
	_redirection.insert(std::pair<int, std::string>(DEFAULT_REDIRECTION_NUM,
													DEFAULT_REDIRECTION_PAGE));
}

Location::Location(std::string path) : _pathLocation(path), _root(DEFAULT_ROOT),
									   _autoindex(false) {
	_acceptedMethods.insert("GET");
	_acceptedMethods.insert("POST");
	_acceptedMethods.insert("DELETE");
	_redirection.insert(std::pair<int, std::string>(DEFAULT_REDIRECTION_NUM,
													DEFAULT_REDIRECTION_PAGE));
}

Location::~Location() {}

Location::Location(const Location &obj) {
	*this = obj;
}

Location &Location::operator=(const Location &obj) {
	if (this != &obj) {
		_pathLocation = obj._pathLocation;
		_acceptedMethods = obj._acceptedMethods;
		_redirection = obj._redirection;
		_root = obj._root;
		_autoindex = obj._autoindex;
		_cgi = obj._cgi;
		_upload = obj._upload;
	}
	return *this;
}

/**
 * SETTERS
 */

void Location::setPathLocation(const std::string &pathLocation) {
	_pathLocation = pathLocation;
}

void Location::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

void Location::setMethods(const std::set<std::string> &acceptedMethods) {
	_acceptedMethods = acceptedMethods;
}

void Location::setRoot(const std::string &root) {
	_root = root;
}

void Location::setRedirection(const std::map<int, std::string> &redirection) {
	_redirection = redirection;
}

void Location::setCGI(const std::pair<std::string, std::string> &cgi) {
	_cgi = cgi;
}
