//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include <vector>
#include <string>
#include "Configurator.hpp"
#include "Server.hpp"
#include "settings.hpp"
#include "utils.hpp"
#include <iostream>

Configurator::Configurator() {}

Configurator::~Configurator() {}


std::pair<std::string, std::string> Configurator::splitDirective(std::string &input) {
	std::string cleanInput = utils::trim(input);
	size_t splitPos = cleanInput.find_first_of(WHITESPACES);

	if (splitPos > input.size()) {
		throw std::runtime_error("Config error: unbalanced directive.");
	}
	std::string directive = cleanInput.substr(0, splitPos);
	std::string directiveValue = cleanInput.substr(splitPos, cleanInput.size());

	return std::pair<std::string, std::string>(utils::trim(directive), utils::trim(directiveValue));
}

// Translate strings to enums to allow work with switch case
Configurator::eDirectives Configurator::resolveDirective(const std::string &input) {
	if (input == "listen") return LISTEN;
	if (input == "server_name") return SERVER_NAME;
	if (input == "error_page") return ERROR_PAGE;
	if (input == "client_max_body_size") return BODY_SIZE;
	if (input == "location") return ROUTE_LOCATION; // Check this
	if (input == "root") return ROOT;
	if (input == "accepted_methods") return ACCEPTED_METHODS;
	if (input == "index") return INDEX;
	if (input == "autoindex") return AUTOINDEX;
	if (input == "cgi") return CGI;
	if (input == "upload") return UPLOAD;
	if (input == "redirection") return REDIRECTION;
	return INVALID;
}

//bool Configurator::_isValidPortRange(const std::string &port) { // testing
//	size_t portNumber;
//	if (!utils::isPositiveNumber(port)) {
//		std::cerr << PURPLE "Config error: port must contain digits only [Check this --> " + port + "]" RESET
//				  << std::endl;
//		return false;
////		throw std::runtime_error("Config error: port must contain digits only [" + port + "]");
//	}
//	portNumber = utils::stringToPositiveNum(port);
//	if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
//		std::cerr << PURPLE "Config error: port number greater than 65535. [[Check this --> " + port + "]" RESET
//				  << std::endl;
//		return false;
////		throw std::runtime_error("Config error: port number greater than 65535. [" + port + "]");
//	}
//	return true;
//}

void Configurator::_checkPortRange(const std::string &port) { // change the name
	size_t portNumber = utils::stringToPositiveNum(port);
	if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
		throw std::runtime_error("Config error: invalid port value - Check this --> '" + port + "'");
	}
}

// Valid ipv4 address 127.127.127.127
bool Configurator::isValidIpv4Address(const std::string &ipAddress) {
	if (ipAddress == "localhost") {
		return true;
	}
	size_t numDots = std::count(ipAddress.begin(), ipAddress.end(), '.');
	std::vector<std::string> vec = utils::splitString(ipAddress, '.');
	if (numDots > 3 || vec.size() != 4) {
		return false;
	}
	size_t num;
	for (size_t i = 0; i < vec.size(); i++) {
		num = utils::stringToPositiveNum(vec.at(i));
		if (num > 255) {
			return false;
		}
	}
	return true;
}

//where do i assign correct values for ip and port??
bool Configurator::isValidIpPort(const std::string &listenValue) {
//	std::string ip;
//	std::string port;
	size_t numColon = std::count(listenValue.begin(), listenValue.end(), ':');
	size_t posColon = listenValue.find_last_of(':');

	if (numColon > 1 || posColon == (listenValue.size() - 1)) {
		return false;
	} else if (numColon == 1) {
		std::vector<std::string> vec = utils::splitString(listenValue, ':');
		bool validIpv4 = Configurator::isValidIpv4Address(vec.at(0));
		if (!validIpv4) {
			return false;
		}
		Configurator::_checkPortRange(vec.at(1));
	}
	return true;
}


//bool isValidIpv4(const std::string& ip){
//	size_t index = 0;
//	size_t dotCounter = 0;
//	size_t numCounter = 0;
//	while (index < ip.size()){
//		if (std::isdigit(ip[index])){
//			size_t start = index;
//			std::skip
//		}
//	}
//}

//void Configurator::_checkServerName(const std::string &serverName) {
//	if (serverName.empty()){
//		Server::
//	}
//}




//
//void Configurator::setServerDirectives(const std::string &rawInput, Server *server) {
//
//	std::pair<std::string, std::string> directive = splitDirective(rawInput);
//
//	switch (resolveDirective(directive.first)) {
//		case LISTEN:
//			server->setListenFromInput(directive.second);
//		case SERVER_NAME:
//			//TODO;
//			break;
//		case ERROR_PAGE:
//			//TODO;
//			break;
//		case BODY_SIZE:
//			//TODO;
//			break;
//		default:
//			throw std::runtime_error("Config error: unknown directive in server block" + rawInput);
//	}
//}

//bool Configurator::setLocationDirectives(const std::string &input) {
//
//	switch (resolveDirective(input)) {
//		case ROUTE_LOCATION:
//			return isValid(value);
//		case ROOT:
//			return ;
//		case ACCEPTED_METHODS:
//			return ;
//		case INDEX:
//			return ;
//		case AUTOINDEX:
//			return ;
//		case CGI:
//			return ;
//		case UPLOAD:
//			return ;
//		case REDIRECTION:
//			return ;
//		default:
//			throw std::runtime_error("Config error: unknown directive in location block" + directive, std::string &value);
//	}
//}
