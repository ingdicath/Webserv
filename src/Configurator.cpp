//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include <vector>
#include <string>
#include <set>
#include <iterator>
#include "Configurator.hpp"
#include "Server.hpp"
#include "settings.hpp"
#include "utils.hpp"
#include <iostream>

Configurator::Configurator() {}

Configurator::~Configurator() {}

//std::pair<std::string, std::string> Configurator::splitDirective2(std::string &input) {
//	std::string cleanInput = utils::trim(input);
//	size_t splitPos = cleanInput.find_first_of(WHITESPACES);
//
//	if (splitPos > input.size()) {
//		throw std::runtime_error("Config error: unbalanced directive.");
//	}
//	std::string directive = cleanInput.substr(0, splitPos);
//	std::string directiveValue = cleanInput.substr(splitPos, cleanInput.size());
//
//	return std::pair<std::string, std::string>(utils::trim(directive), utils::trim(directiveValue));
//}


//option 2 for split directives
std::pair<std::string, std::vector<std::string> > Configurator::splitDirective(std::string &input) {
	std::string cleanInput = utils::trim(input);
	size_t splitPos = cleanInput.find_first_of(WHITESPACES);

	if (splitPos > input.size()) { // check if key and value exists
		throw std::runtime_error("Config error: unbalanced directive Key.");
	}
	std::string directiveKey = cleanInput.substr(0, splitPos);
	std::string directiveValue = cleanInput.substr(splitPos, cleanInput.size());
	std::vector<std::string> directiveValues;

	std::cout << "directive: " << directiveKey << std::endl; //delete
	while (true) {
		directiveValue = utils::trim(directiveValue);
		splitPos = directiveValue.find_first_of(WHITESPACES);

		std::string value = directiveValue.substr(0, splitPos);
		directiveValues.push_back(value);
		if (splitPos > directiveValue.size()) {
			break;
		}
		directiveValue = directiveValue.substr(splitPos, directiveValue.size());
	}

	//delete
	for (size_t i = 0; i < directiveValues.size(); i++) {
		std::cout << "values are at: " << i << " " << directiveValues.at(i) << std::endl;
	}
//	_isValidListenValues(directiveValues); //delete
	_isValidErrorPageConfig(directiveValues);

	return std::pair<std::string, std::vector<std::string> >(utils::trim(directiveKey), directiveValues);
}

//std::vector<std::string> Configurator::splitDirectiveValues(std::string &input) {
//	std::string cleanInput = utils::trim(input);
//	size_t splitPos = cleanInput.find_first_of(WHITESPACES);
//
//	if (splitPos > input.size()) {
//		throw std::runtime_error("Config error: unbalanced directive.");
//	}
//	std::string directive = cleanInput.substr(0, splitPos);
//	std::string directiveValue = cleanInput.substr(splitPos, cleanInput.size());
//
//	return std::pair<std::string, std::string>(utils::trim(directive), utils::trim(directiveValue));
//}


void printSet(const std::set<std::string> mySet){
	std::cout << "myset contains:";
	for (auto it = mySet.begin(); it != mySet.end(); ++it) {
		std::cout << ' ' << *it;
	}
	std::cout << std::endl;
}


/**
 * LISTEN
 */
bool Configurator::_isValidPortRange(const std::string &port) {
	size_t portNumber = utils::stringToPositiveNum(port);
	if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
		throw std::runtime_error("Config error: invalid port value: '" + port + "'");
	}
	return true;
}

// Valid ipv4 address 127.127.127.127
bool Configurator::_isValidIpv4Address(const std::string &ipAddress) {
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
bool Configurator::_isValidIpPort(const std::string &listenValue) {
//	std::string ip;
//	std::string port;
	size_t numColon = std::count(listenValue.begin(), listenValue.end(), ':');
	size_t posColon = listenValue.find_last_of(':');

	if (numColon > 1 || posColon == (listenValue.size() - 1)) {
		return false;
	} else if (numColon == 1) {
		std::vector<std::string> vec = utils::splitString(listenValue, ':');
		bool valid = Configurator::_isValidIpv4Address(vec.at(0)) &&
					 Configurator::_isValidPortRange(vec.at(1));
		return valid;
	} else {
		bool valid = Configurator::_isValidIpv4Address(listenValue) ||
					 Configurator::_isValidPortRange(listenValue);
		return valid;
	}
}

bool Configurator::_isValidListenValues(std::vector<std::string> values) {
	std::set<std::string> mySet;
	for (size_t i = 0; i < values.size(); i++) {
		if (!_isValidIpPort(values[i])) {
			throw std::runtime_error("Config error: invalid listen values.");
		}
		if (!mySet.insert(values[i]).second) {
			throw std::runtime_error("Config error: duplicate value in listen.");
		}
	}

	printSet(mySet); 	// delete

	return true;
}


//bool Configurator::_isValidPortRange(std::vector<std::string> values) {
//	size_t portNumber;
//	std::set<std::size_t> setPort;
//	for (size_t i = 0; i < values.size(); i++) {
//		portNumber = utils::stringToPositiveNum(values[i]);
//		if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
//			throw std::runtime_error("Config error: invalid port value.");
//		}
//		if (!setPort.insert(portNumber).second) {
//			throw std::runtime_error("Config error: duplicate value in listen.");
//		}
//	}
//
//	// delete
//	std::cout << "myset contains:";
//	for (auto it = setPort.begin(); it != setPort.end(); ++it) {
//		std::cout << ' ' << *it;
//	}
//	std::cout << std::endl;
//
//	return true;
//}




/**
 * ERROR PAGES
 */


bool Configurator::_isValidErrorPageConfig(std::vector<std::string> values) {
	if (values.size() != 2) {
		throw std::runtime_error("Config error: invalid args for error pages directive.");
	}
	if (!_isValidErrorCode(values[0])) {
		throw std::runtime_error("Config error: invalid path.");
	}
	if (!_isValidPath(values[1])) {
		throw std::runtime_error("Config error: invalid path.");
	}
	std::set<std::string> mySet;
	for (size_t i = 0; i < values.size(); ++i) {
		if (!mySet.insert(values[i]).second) {
			throw std::runtime_error("Config error: duplicate value in listen.");
		}
	}

	printSet(mySet); 	// delete

	return true;
}


// check if this validation is necessary
bool Configurator::_isValidPath(const std::string &path) {
	if (path[0] != '/') {
		return false;
	}
	if (path.find_last_of('/') == path.size() - 1 && path.size() != 1) {
		throw std::runtime_error("Config error: path can't be a directory.");
	}
	return true;
}

// max possible error code is 505
bool Configurator::_isValidErrorCode(const std::string &string) {
	if (!utils::isPositiveNumber(string)) {
		return false;
	}
	size_t errorCode = utils::stringToPositiveNum(string);
	if (errorCode < 300 || errorCode > 506) {
		throw std::runtime_error("Config error: Invalid error code.");
	}
	return true;
}

bool Configurator::_isValidErrorPageConfig(std::string &string) {
	std::string cleanInput = utils::trim(string);
	size_t splitPos = cleanInput.find_first_of(WHITESPACES);

	if (splitPos > string.size()) {
		return false;
	}
	std::string errorCode = cleanInput.substr(0, splitPos);
	std::string errorPath = cleanInput.substr(splitPos, cleanInput.size());
	errorPath = utils::lTrim(errorPath);
	if (!Configurator::_isValidErrorCode(errorCode) || !Configurator::_isValidPath(errorPath)) {
		return false;
	}
	return true;
}

/**
 * BODY SIZE
 * how check overflow client_body_size?
 * Is 0 a valid value for body size?
 * Will we predefine a max body size client?
 */
bool Configurator::_isValidBodySize(std::string &string) {
	std::string cleanInput = utils::trim(string);

	std::cout << "clean input is 1: " << cleanInput << std::endl;  //delete
	if (string.find_first_of("KMGkmg") != string.size() - 1) {
		return false;
	}
	cleanInput[cleanInput.size() - 1] = std::toupper(
			cleanInput[cleanInput.size() - 1]); //convert to uppercase last letter
	std::cout << "clean input is 2: " << cleanInput << std::endl; //delete

	if (cleanInput[cleanInput.size() - 1] == 'K') {
		cleanInput = utils::deleteLastOf('K', cleanInput);
	} else if (cleanInput[cleanInput.size() - 1] == 'M') {
		cleanInput = utils::deleteLastOf('M', cleanInput);
	} else {
		cleanInput = utils::deleteLastOf('G', cleanInput);
	}
	std::cout << "clean input is 3: " << cleanInput << std::endl; //delete

//	if (!utils::isPositiveNumber(cleanInput)){
//		throw std::logic_error("Config error: invalid client max body size.");
//	}
	size_t bodySize = utils::stringToPositiveNum(cleanInput);
	if (bodySize <= 0 || bodySize > DEFAULT_CLIENT_MAX_BODY_SIZE) {
		throw std::out_of_range("Config error: invalid client max body size.");
	}
	return true;
}


/**
 * ROOT - check this
 */
// https://www.youtube.com/watch?v=ewqX1IuYzC8 for realpath explanation

bool Configurator::_isValidRoot(std::string &string) {
	std::string cleanInput = utils::trim(string);
	if (cleanInput.find_last_of('/') == cleanInput.size() - 1 && cleanInput.size() != 1) {
		throw std::runtime_error("Config error: root can't be a directory.");
	}
	// resolve a pathname - get the absolute path of a file
	if (cleanInput[0] != '/') {
		char realPath[4096];
		realpath(cleanInput.c_str(), realPath);
		cleanInput = realPath;
		std::cout << "cleanInput is : " << cleanInput << std::endl;
		std::cout << "real path is: " << realPath << std::endl;
	}
	return true;
}

/**
 * ALLOWED METHODS
 */

bool Configurator::_isValidAllowedMethod(std::string string) {
	std::string cleanInput = utils::trim(string);

	size_t splitPos = cleanInput.find_first_of(WHITESPACES);
	std::cout << "splitPos is : " << splitPos << std::endl;
	std::cout << "cleanInput is: " << cleanInput << std::endl;
	std::cout << "cleanInput.size() is: " << cleanInput.size() << std::endl;

	if (splitPos > cleanInput.size()) {
		return false;
	}

	return true;
}









//void Configurator::_checkServerName(const std::string &serverName) {
//	if (serverName.empty()){
//		Server::
//	}
//}


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
