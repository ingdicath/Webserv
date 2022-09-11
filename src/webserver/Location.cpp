//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include <csignal> //for getcwd Linux
#include <unistd.h>
#include "Location.hpp"
#include "../utils/settings.hpp"

Location::Location() : _pathLocation("/"),
					   _index(DEFAULT_INDEX), _autoindex(false) {
	_setDefaultAllowedMethods();
	_setDefaultRedirectionPages();
	_setDefaultRoot();
}

Location::Location(std::string path) : _pathLocation(path),
									   _index(DEFAULT_INDEX), _autoindex(false) {
	_setDefaultAllowedMethods();
	_setDefaultRedirectionPages();
	_setDefaultRoot();
}

Location::~Location() {}

Location::Location(const Location &obj) : _autoindex() {
	*this = obj;
}

Location &Location::operator=(const Location &obj) {
	if (this != &obj) {
		_pathLocation = obj._pathLocation;
		_acceptedMethods = obj._acceptedMethods;
		_redirection = obj._redirection;
		_root = obj._root;
		_index = obj._index;
		_autoindex = obj._autoindex;
		_cgi = obj._cgi;
		_upload = obj._upload;
	}
	return *this;
}

/**
 * SETTERS
 */

void Location::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

void Location::setMethods(const std::set<std::string> &acceptedMethods) {
	_acceptedMethods = acceptedMethods;
}

void Location::setRoot(const std::string &root) {
	_root = root;
}

void Location::setCGI(const std::pair<std::string, std::string> &cgi) {
	_cgi = cgi;
}

void Location::setUpload(const std::string &upload) {
	_upload = upload;
}

void Location::setIndex(const std::string &index) {
	_index = index;
}

void Location::_setDefaultRedirectionPages() {
	_redirection.insert(std::pair<int, std::string>(301, "/redirection/301.html"));
	_redirection.insert(std::pair<int, std::string>(302, "/redirection/302.html"));
	_redirection.insert(std::pair<int, std::string>(303, "/redirection/303.html"));
	_redirection.insert(std::pair<int, std::string>(307, "/redirection/307.html"));
}

void Location::_setDefaultAllowedMethods() {
	_acceptedMethods.insert("GET");
	_acceptedMethods.insert("POST");
	_acceptedMethods.insert("DELETE");
}

void Location::_setDefaultRoot() {
	char cwd[MAXLINE];
	char *res = getcwd(cwd, sizeof(cwd));
	_root = res;
	_root = _root.append(DEFAULT_ROOT);
}

void Location::addRedirection(const std::pair<int, std::string> &redirection) {
	std::map<int, std::string>::iterator it = _redirection.find(redirection.first);
	if (it != _redirection.end()) {
		_redirection.at(it->first) = redirection.second;
	} else {
		_redirection.insert(redirection);
	}
}
