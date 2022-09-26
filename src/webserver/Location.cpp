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
	if (_autoindex == true)
		_index = "";
	else
		_index = index;
}

void Location::setRedirection(const std::pair<int, std::string> &redirection) {
	_redirection = redirection;
}
void Location::_setDefaultRedirectionPages() {
	_redirection = std::make_pair(-1, "");
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

/**
 * GETTERS
 */

const std::string &Location::getPathLocation() const {
	return _pathLocation;
}
const std::string &Location::getUpload() const {
	return _upload;
}
const std::string &Location::getRoot() const {
	return _root;
}
const std::string &Location::getIndex() const {
	return _index;
}
bool Location::isAutoindex() const {
	return _autoindex;
}
const std::set<std::string> &Location::getAcceptedMethods() const {
	return _acceptedMethods;
}

const std::pair<std::string, std::string> &Location::getCgi() const {
	return _cgi;
}
const std::pair<int, std::string> Location::getRedirection() const {
	return _redirection;
}
