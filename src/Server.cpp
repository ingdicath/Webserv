//
// Created by Diana catherine Salamanca leguizamon on 5/24/22.
//

#include "Server.hpp"
#include "settings.hpp"
#include "utils.hpp"
#include "Configurator.hpp"

Server::Server() : _client_max_body_size(DEFAULT_CLIENT_MAX_BODY_SIZE) {

}

Server::~Server() {

}

Server::Server(const Server &obj) : _client_max_body_size(DEFAULT_CLIENT_MAX_BODY_SIZE) {
	*this = obj;
}

Server &Server::operator=(const Server &obj) {
	if (this != &obj) {
		_listen = obj._listen;
		_server_name = obj._server_name;
		_error_page = obj._error_page;
		_client_max_body_size = obj._client_max_body_size;
		_locations = obj._locations;
	}
	return *this;
}

const std::vector<std::string> &Server::getServerName() const {
	return _server_name;
}

Server::Server(Server *pServer) {
}

void Server::validateAndSetListen(std::vector<std::string> values) {
	if (Configurator::_isValidListenValues(values)) {
		std::set<std::string> mySet;
		for (size_t i = 0; i < values.size(); i++) {
			mySet.insert(values[i]);
		}
		_listen = mySet;
	}
}

const std::vector<Location> &Server::getLocations() const {
	return _locations;
}

const std::set<std::string> &Server::getListen() const {
	return _listen;
}

const std::map<std::size_t, std::string> &Server::getErrorPage() const {
	return _error_page;
}

size_t Server::getClientMaxBodySize() const {
	return _client_max_body_size;
}

void Server::setLocations(const std::vector<Location> &locations) {
	_locations = locations;
}

void Server::setListen1(const std::set<std::string> &listen) {
	_listen = listen;
}

void Server::setServerName(const std::vector<std::string> &serverName) {
	_server_name = serverName;
}

void Server::setErrorPage(const std::map<std::size_t, std::string> &errorPage) {
	_error_page = errorPage;
}

void Server::setClientMaxBodySize(size_t clientMaxBodySize) {
	_client_max_body_size = clientMaxBodySize;
}


//
//void Server::setListenFromInput(const std::string &str) {
//
//
//
//}


//// https://stackoverflow.com/questions/18677171/throwing-exception-when-the-same-key-inserted-into-stdmap
//void Server::setListen(const std::string& str) {
//	if (!_listen.insert(Server::_isValidPortRange(str)).second){
//		//check if is better create a specific class for every exception
//		throw std::invalid_argument("Config error: duplicate " + str);
//	}
//}
