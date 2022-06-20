//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include <vector>
#include <string>
#include <set>
#include <iterator>
#include <algorithm>
#include "Configurator.hpp"
#include "Server.hpp"
#include "settings.hpp"
#include "utils.hpp"
#include <iostream>

Configurator::Configurator() {}

Configurator::~Configurator() {}

Directive Configurator::splitDirective(std::string &input) {
	std::string cleanInput = utils::trim(input);
	size_t splitPos = cleanInput.find_first_of(WHITESPACES);

	// check if key and value exists
	if (splitPos > input.size()) {
		throw std::runtime_error("Config error: unbalanced directive Key.");
	}
	std::string directiveKey = cleanInput.substr(0, splitPos);
	std::string directiveValue = cleanInput.substr(splitPos, cleanInput.size());
	std::vector<std::string> directiveValues;

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
//	for (size_t i = 0; i < directiveValues.size(); i++) {
//		std::cout << "values are at: " << i << " " << directiveValues.at(i) << std::endl;
//	}
//	_isValidListenValues(directiveValues); //delete
//	_isValidErrorPageConfig(directiveValues); //delete
//	_isValidAllowedMethod(directiveValues); //delete
//	_isValidBodySize(directiveValues); // delete
//	_isValidRoot(directiveValues); //delete
	_isValidIndex(directiveValues);//delete
	Directive directive;
	directive.key = utils::trim(directiveKey);
	directive.value = directiveValues;
//(utils::trim(directiveKey),directiveValues);

//	Directive directive2(utils::trim(directiveKey),directiveValues);
	return directive;
}

//delete, used for testing purposes
void printSet(const std::set<std::string> mySet) {
	std::cout << "my set contains:";
//	std::set<int>::iterator it;
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
	return true;
}


/**
 * ERROR PAGES
 */

bool Configurator::_isValidErrorPageConfig(std::vector<std::string> values) {
	if (values.size() != 2) {
		throw std::runtime_error("Config error: invalid args for error pages directive.");
	}
	if (!_isValidErrorCode(values[0])) {
		throw std::runtime_error("Config error: invalid error code.");
	}
	if (!_isValidPath(values[1])) {
		throw std::runtime_error("Config error: invalid path.");
	}
	std::set<std::string> mySet;
	for (size_t i = 0; i < values.size(); ++i) {
		if (!mySet.insert(values[i]).second) {
			throw std::runtime_error("Config error: duplicate value in Error page.");
		}
	}
	printSet(mySet); //delete
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


/**
 * BODY SIZE
 * how check overflow client_body_size?
 * Is 0 a valid value for body size?
 * Will we predefine a max body size client?
 * Remove or not remove last character here? Put another validation??
 */

bool Configurator::_isValidBodySize(std::vector<std::string> values) {
	if (values.size() != 1) {
		throw std::runtime_error("Config error: invalid args for body_size directive.");
	}

	size_t lastPos = values[0].size() - 1;
	if (values[0].find_first_of("KMGkmg") != lastPos) {
		throw std::runtime_error("Config error: invalid measure for body_size directive.");
	}
	values[0].at(lastPos) = char(std::toupper(static_cast<unsigned char>(values[0].at(lastPos))));

	if (values[0].at(lastPos) == 'K') {
		// set here value, e.g. * 10000
		values[0] = utils::deleteLastOf('K', values[0]);
	} else if (values[0].at(lastPos) == 'M') {
		// set here value, e.g. * 10000
		values[0] = utils::deleteLastOf('M', values[0]);
	} else {
		// set here value, e.g. * 100000000
		values[0] = utils::deleteLastOf('G', values[0]);
	}

//	std::set<std::string> mySet;
//	for (size_t i = 0; i < values.size(); ++i) {
//		if (!mySet.insert(values[i]).second) {
//			throw std::runtime_error("Config error: duplicate value in client body size.");
//		}
//	}
//	printSet(mySet); //delete
	return true;
}


/**
 * ROOT - check this
 */
// https://www.youtube.com/watch?v=ewqX1IuYzC8 for realpath explanation

bool Configurator::_isValidRoot(std::vector<std::string> values) {
	if (values.size() != 1) {
		throw std::runtime_error("Config error: invalid args for root.");
	}

	size_t lastPos = values[0].size() - 1;
	if (values[0].find_last_of('/') == lastPos && values[0].size() != 1) {
		throw std::runtime_error("Config error: root can't be a directory.");
	}
	// resolve a pathname - get the absolute path of a file, check if is necessary
	if (values[0].at(0) != '/') {
		char realPath[4096];
		realpath(values[0].c_str(), realPath);
		values[0] = realPath;
		std::cout << "cleanInput is : " << values[0] << std::endl; //delete
		std::cout << "real path is: " << realPath << std::endl; //delete
	}
	return true;
}

//bool Configurator::_isValidRoot(std::string &string) {
//	std::string cleanInput = utils::trim(string);
//	if (cleanInput.find_last_of('/') == cleanInput.size() - 1 && cleanInput.size() != 1) {
//		throw std::runtime_error("Config error: root can't be a directory.");
//	}
//	// resolve a pathname - get the absolute path of a file
//	if (cleanInput[0] != '/') {
//		char realPath[4096];
//		realpath(cleanInput.c_str(), realPath);
//		cleanInput = realPath;
//		std::cout << "cleanInput is : " << cleanInput << std::endl;
//		std::cout << "real path is: " << realPath << std::endl;
//	}
//	return true;
//}

/**
 * ALLOWED METHODS
 */

bool Configurator::_isValidAllowedMethod(std::vector<std::string> values) {
	std::set<std::string> mySet;
	for (size_t i = 0; i < values.size(); i++) {
		values[i] = utils::stringToUpper(values.at(i));
		if (!mySet.insert(values[i]).second) {
			throw std::runtime_error("Config error: duplicate value in allowed_method directive. '" + values[i] + "'");
		}
		if (values[i] != "GET" && values[i] != "POST" && values[i] != "DELETE") {
			throw std::runtime_error("Config error: invalid value in allowed_method directive. '" + values[i] + "'");
		}
	}
	printSet(mySet); //delete
	return true;
}

/**
 * INDEX
 */

bool Configurator::_isValidIndex(std::vector<std::string> values) {
	size_t lastPos = values[0].size() - 1;
	if (values[0].find_last_of('/') == lastPos && values[0].size() != 1) {
		throw std::runtime_error("Config error: index can't be a directory.");
	}
	std::string extension = values[0].substr(values[0].find_last_of('.') + 1);
	extension = utils::stringToLower(extension);
	if (extension != "html")
		throw std::runtime_error("Config error: index can only has a html extension");
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
