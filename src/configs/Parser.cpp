//
// Created by Diani on 08/08/2022.
//

#include <iostream>
#include <algorithm>
#include "Parser.hpp"
#include "FileUtils.hpp"
#include "../utils/utils.hpp"

Parser::Parser() {}

Parser::~Parser() {}


/**
 * In this function we are loading the configuration file and validate it char by char.
 * @param configFile is the path where is located the configuration file.
 */

// TODO: Check if is possible to create server and location without using keyword new
std::vector<Server> Parser::validateConfiguration(const std::string &configFile) {
	std::vector<Server> servers;
	std::string line;
	char currentChar;
	bool isComment = false;
	std::stack<std::string> sectionBlock; //to validate server and location keyword
	FileUtils fileUtils;
	fileUtils.openFile(_file, configFile);

	while (_file.get(currentChar)) {
		switch (currentChar) {
			case '{':
				_checkOpenCurly(isComment, &sectionBlock, &servers, line);
				line = "";
				break;
			case ';':
				_checkSemiColon(isComment, &servers, line);
				line = "";
				break;
			case '}':
				_checkCloseCurly(isComment, &sectionBlock);
				break;
			case '#':
				isComment = true;
				break;
			case '\n':
				isComment = false;
				break;
			default:
				if (!isComment)
					line += currentChar;
				break;
		}
	}
	if (!sectionBlock.empty()) {
		//TODO: fix cleanServerBlocks to avoid memory leaks
		cleanServerBlocks(&servers);
		throw ConfigFileException("open unbalanced curly braces.");
	}
	fileUtils.closeFile(_file);
	return servers;
}

/**
 *
 * @param isComment validates if a line is a comment or not.
 * @param sectionBlock allow to store keyword server and location in a stack.
 * @param serverBlocks is the vector where we are adding server set up in the config file.
 * @param line is the line we are reading.
 */
void Parser::_checkOpenCurly(bool isComment, std::stack<std::string> *sectionBlock,
							 std::vector<Server> *serverBlocks,
							 std::string line) {
	if (!isComment) {
		line = utils::trim(line);
		int postPath = static_cast<int>(line.find_first_of('/'));
		std::string command = line.substr(0, postPath);
		command = utils::trim(command);

		if (sectionBlock->empty() && line == "server") {    // See if it is better define as enum
			serverBlocks->push_back(*new Server());        // BE CAREFULL: if you use 'new' you should delete as well
		} else if (sectionBlock->empty() && command == "location") {
			//TODO: fix cleanServerBlocks to avoid memory leaks
			cleanServerBlocks(serverBlocks);
//			while (1) {}; //delete, just testing for memory leaks
			throw ConfigFileException("location block found outside server block.");
		} else if (sectionBlock->top() == "server" && command == "location" && postPath != -1) {
			std::string pathLocation = line.substr(postPath - 1, line.size());
			pathLocation = utils::trim(pathLocation);
			Server &server = serverBlocks->back();  //brings last server to create new location
			Location location = *new Location(); // BE CAREFULL: if you use 'new' you should delete as well
			location.setPathLocation(pathLocation);
			server.addLocation(location);
		} else if (sectionBlock->top() == "location" && command == "location") {
			//TODO: fix cleanServerBlocks to avoid memory leaks
			cleanServerBlocks(serverBlocks);
//			while (1) {}; //delete, just testing for memory leaks
			throw ConfigFileException("location block unclosed.");
		} else {
//			//TODO: fix cleanServerBlocks to avoid memory leaks
			cleanServerBlocks(serverBlocks);
			throw ConfigFileException("invalid value, no server or location detected in open block.");
		}
		sectionBlock->push(command);
		std::cout << "curly: " << sectionBlock->top() << std::endl;
	}
}

/**
 * When the line reaches a semicolon (;), it is stored in a directive data structure
 * composed by two parts: key and value(e.g. key-> port, value-> 80).
 * Then, this directive is stored in the last server added in the vector of servers.
 */
void Parser::_checkSemiColon(bool isComment, std::vector<Server> *serverBlocks, std::string line) {
	if (!isComment) {
		line = utils::trim(line);
		std::cout << "line that is validated: " << line << std::endl; //test, delete
		Directive directive = Parser::_splitDirective(line);
		_storeDirective(directive, &serverBlocks->back());
	}
}

/**
 * When the line reaches a close curly brace, it is verified if there is a server
 * or location block opened.
 */
void Parser::_checkCloseCurly(bool isComment, std::stack<std::string> *sectionBlock) {
	if (!isComment) {
		if (sectionBlock->empty()) {
			throw ConfigFileException("unbalanced curly braces.");
		}
		sectionBlock->pop();
	}
}

/**
 * When it is found a key that corresponds to a valid parameter, it will be validated
 * and stored in the Server or in the location class parameters.
 * @param directive
 * @param server
 */
void Parser::_storeDirective(Directive directive, Server *server) {
	switch (Parser::_resolveDirective(directive._key)) {
		case PORT:
			server->setPort(_checkPort(directive._value[0]));
			break;
		case HOST_:
			server->setHost(_checkHost(directive._value[0]));
			break;
		case SERVER_NAME:
			server->setServerName(_checkServerNames(directive._value));
			break;
//		case ERROR_PAGE:
//		server->validateAndSetErrorPages(directive.value);
//			break;
//		case BODY_SIZE:
//			//TODO
//			break;
//		case ROUTE_LOCATION:
//			//TODO
//			break;
//		case ROOT:
//			//TODO
//			break;
//		case ACCEPTED_METHODS:
//			//TODO
//			break;
//		case INDEX:
//			//TODO
//			break;
//		case AUTOINDEX:
//			//TODO
//			break;
//		case CGI:
//			//TODO
//			break;
//		case UPLOAD:
//			//TODO
//			break;
//		case REDIRECTION:
//			//TODO
//			break;
//		case INVALID:
//			//TODO, error message
		default:
			break;
	}
}

/**
 * Translate strings to enums to allow work with switch case
 */
Parser::eDirectives Parser::_resolveDirective(const std::string &input) {
	if (input == "port") return Parser::PORT;
	if (input == "host") return Parser::HOST_;
	if (input == "server_name") return Parser::SERVER_NAME;
	if (input == "error_page") return Parser::ERROR_PAGE;
	if (input == "client_max_body_size") return Parser::BODY_SIZE;
	if (input == "location") return Parser::ROUTE_LOCATION; // Check this
	if (input == "root") return Parser::ROOT;
	if (input == "accepted_methods") return Parser::ACCEPTED_METHODS;
	if (input == "index") return Parser::INDEX;
	if (input == "autoindex") return Parser::AUTOINDEX;
	if (input == "cgi") return Parser::CGI;
	if (input == "upload") return Parser::UPLOAD;
	if (input == "redirection") return Parser::REDIRECTION;
	return Parser::INVALID;
}

/**
 *
 * @param input Refers to the line that contains keyword (e.g. port) and
 * its value (e.g. 80)
 */
Parser::Directive Parser::_splitDirective(std::string &input) {
	std::string cleanInput = utils::trim(input);
	size_t splitPos = cleanInput.find_first_of(WHITESPACES);

	// check if key and value exists
	if (splitPos > input.size()) {
		throw ConfigFileException("unbalanced directive Key.");
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
	Directive directive;
	directive._key = utils::trim(directiveKey);
	directive._value = directiveValues;
	return directive;
}

bool Parser::_isValidPortRange(const std::string &port) {
	bool res = true;
	size_t portNumber = utils::stringToPositiveNum(port);
	if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
		res = false;
	}
	return res;
}

/**
 * A valid value for port can be 'localhost' or an ip4 address like '127.0.0.0'
 */
bool Parser::_isValidIpv4Address(const std::string &ipAddress) {
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

/**
 * @param serverName One server name to be validated.
 */
bool Parser::_isValidServerName(std::string serverName) {
	serverName = utils::trim(serverName);
	if (serverName[0] == '/' || serverName[0] == '*') {
		std::cerr << RED "Path can't be a directory or use wildcard *. " RESET << std::endl;
		return false;
	}
	if (serverName.find_last_of('/') == serverName.size() - 1 && serverName.size() != 1) {
		std::cerr << RED "Path can't be a directory." RESET << std::endl;
		return false;
	}
	return true;
}

/**
 * @param serverNames vector of server names to be evaluated.
 */
bool Parser::_isValidServerNames(const std::vector<std::string> &serverNames) {
	if (serverNames.empty()) {
		std::cerr << RED "missing argument for server name. " RESET << std::endl;
		return false;
	}
	for (size_t i = 0; i < serverNames.size(); i++) {
		if (!_isValidServerName(serverNames.at(i))) {
			std::cerr << RED "wrong syntax in server name." RESET << std::endl;
			return false;
		}
	}
	return true;
}

//TODO: complete this function
void Parser::cleanServerBlocks(std::vector<Server> *serverBlocks) {
	std::vector<Server>::iterator serverIt = serverBlocks->begin();
	for (; serverIt < serverBlocks->end(); serverIt++) {
		cleanLocationBlocks(serverIt->getLocations());
		std::cout << "Element " << serverIt->getPort() << " erased" << std::endl; // test: delete later
		serverIt = serverBlocks->erase(serverIt);
	}
}

//TODO: complete this function
void Parser::cleanLocationBlocks(std::vector<Location> *locationBlocks) {
	std::vector<Location>::iterator locationIt = locationBlocks->begin();
	for (; locationIt < locationBlocks->end(); locationIt++) {
		locationIt = locationBlocks->erase(locationIt);
	}
}


int Parser::_checkPort(const std::string &port) {
	if (!Parser::_isValidPortRange(port)) {
		throw ConfigFileException("invalid port value: '" + port + "'");
	}
	return utils::strToInt(port);
}

std::string Parser::_checkHost(std::string host) {
	if (!Parser::_isValidIpv4Address(host)) {
		throw ConfigFileException("invalid host value: '" + host + "'");
	}
	if (host == "localhost") {
		host = "127.0.0.1";
	}
	return host;
}

std::vector<std::string> Parser::_checkServerNames(std::vector<std::string> serverNames) {
	if (!_isValidServerNames(serverNames)) {
		throw ConfigFileException("invalid server name value(s)");
	}
	std::vector<std::string> myVector;
	for (size_t i = 0; i < serverNames.size(); i++) {
		myVector.push_back(serverNames[i]);
	}
	return myVector;
}




/*
// ADD DESCRIPTION
void Server::validateAndSetServerNames(std::vector<std::string> values) {
	if (config::_isValidServerNames(values)) {
		std::vector<std::string> myVector; // ana is using port as a number, check if we need to use atoi
		for (size_t i = 0; i < values.size(); i++) {
			myVector.push_back(values[i]);
		}
		_server_name = myVector;

		// delete this, testing
		// for (auto it = _server_name.begin(); it != _server_name.end(); it++) {
		// 	std::cout << *it << " ";
		// }
		//std::cout << std::endl;
	}
}
 */

/**
 * This is a custom exception to show error messages when the configuration file is processed.
 */
Parser::ConfigFileException::ConfigFileException(const std::string &message)
		: msg_("Config error: " + message) {}

Parser::ConfigFileException::~ConfigFileException() throw() {}

const char *Parser::ConfigFileException::what() const throw() {
	return msg_.c_str();
}
