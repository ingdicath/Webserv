//
// Created by Diana catherine Salamanca leguizamon on 5/24/22.
//

#include "Server.hpp"
#include "settings.hpp"
#include "utils.hpp"

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

void Server::_checkPortRange(const std::string& port) {
	size_t portNumber;
	if (!utils::isPositiveNumber(port)){
		throw std::runtime_error("Config error: port must contain digits only " + port);
	}
	portNumber = utils::stringToNum(port);
	if (portNumber > MAX_PORT_NUMBER){
		throw std::runtime_error("Config error: port number greater than 65535." + port);
	}
}



//// https://stackoverflow.com/questions/18677171/throwing-exception-when-the-same-key-inserted-into-stdmap
//void Server::setListen(const std::string& str) {
//	if (!_listen.insert(Server::_checkPortRange(str)).second){
//		//check if is better create a specific class for every exception
//		throw std::invalid_argument("Config error: duplicate " + str);
//	}
//}

