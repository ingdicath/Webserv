//
// Created by Diani on 08/08/2022.
//

#include <iostream>
#include "Parser.hpp"
#include "FileUtils.hpp"
#include "../utils/utils.hpp"

/**
 * Load the file and validate char by char.
 */
std::vector<Server> Parser::validateConfiguration(const std::string &configFile) {
	FileUtils fileUtils;
	fileUtils.openFile(_file, configFile);

	std::vector<Server> servers;
	std::string line;
	char currentChar;
	bool isComment = false;
	int curlyCounter = 0;

	while (_file.get(currentChar)) {
		switch (currentChar) {
			case '{':
				checkOpenCurly(servers, isComment, line, &curlyCounter);
				line = "";
				break;
			case ';':
				checkSemiColon(line, isComment, servers);
				line = "";
				break;
			case '}':
				checkCloseCurly(isComment, &curlyCounter, servers);
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
	fileUtils.closeFile(_file);
	return servers;
}

void Parser::checkOpenCurly(std::vector<Server> serverBlocks, bool isComment, std::string line, int *curlyCounter) {
	if (!isComment) {
		*curlyCounter += 1;
		if (*curlyCounter > 2) {
			//TODO: create function clean and throw error
			cleanServerBlocks(serverBlocks);
			throw std::runtime_error("Config error: unbalanced curly braces.");
		}
		std::cout << "curly: " << line << std::endl;
		line = utils::trim(line);
		if (line == "server") {                    // See if it is better define as enum
			serverBlocks.push_back(*new Server());        // BE CAREFULL: if you use 'new' you should delete as well
			return;
		}
		// TODO extraer location
		size_t postPath = line.find_first_of('/'); //what does happen if there is no '/'??
		std::string cmdLocation = line.substr(0, postPath);
		std::string pathLocation = line.substr(postPath - 1, line.size());

		if (cmdLocation == "location") {
			Server &server = serverBlocks.back();  //brings last server to create new location
			Location location = new Location(); // BE CAREFULL: if you use 'new' you should delete as well
			location.setPathLocation(pathLocation);
			server.getLocations().push_back(location);
			return;
		}
		//TODO:
		std::cout << "here" << std::endl; //delete thiss
		cleanServerBlocks(serverBlocks); // check this
//		throw std::runtime_error("No server or location detected."); // update later
	}
}

void Parser::checkSemiColon(std::string line, bool isComment, std::vector<Server> serverBlocks) {
	if (!isComment) {
		std::cout << "line that is validated: " << line << std::endl;
		Directive directive = Parser::splitDirective(line);
		storeDirective(directive, &serverBlocks.back()); // WATCH OUT! CHECK THIS
	}
}

void Parser::checkCloseCurly(bool isComment, int *pInt, std::vector<Server> serverBlocks) {
	if (!isComment) {
		*pInt -= 1;
		if (*pInt < 0) {
			throw std::runtime_error("Config error: unbalanced curly braces.");
		}
	}
}

void Parser::storeDirective(Directive directive, Server *server) {

	switch (Parser::resolveDirective(directive._key)) {
		case PORT:
			std::cout << "hello port" << std::endl;
//			server->validateAndSetListen(directive._value);
			break;
//		case HOST_:
//			std::cout << "hello host" << std::endl;
//			server->validateAndSetListen(directive._value);
//			break;
//		case SERVER_NAME:
//			std::cout << "hello server name" << std::endl;
//			server->validateAndSetServerNames(directive._value);
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
Parser::eDirectives Parser::resolveDirective(const std::string &input) {
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

Parser::Directive Parser::splitDirective(std::string &input) {
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
	Directive directive;
	directive._key = utils::trim(directiveKey);
	directive._value = directiveValues;
	return directive;
}

bool Parser::_isValidPortRange(const std::string &port) {
	size_t portNumber = utils::stringToPositiveNum(port);
	if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
		throw InvalidPortRangeException();
//		throw std::runtime_error("Config error: invalid port value: '" + port + "'");
	}
	return true;
}


/**
 * Functions to validate parameters in config file
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

bool Parser::_isValidServerName(const std::string &serverName) {
	if (serverName[0] == '/' || serverName[0] == '*') {
		throw std::runtime_error("Config error: path can't be a directory or use wildcard *. ");
	}
	if (serverName.find_last_of('/') == serverName.size() - 1 && serverName.size() != 1) {
		throw std::runtime_error("Config error: path can't be a directory.");
	}
	return true;
}


void Parser::cleanServerBlocks(std::vector<Server> serverBlocks) {
	std::vector<Server>::iterator it = serverBlocks.begin();
	for (; it != serverBlocks.end(); it++) {
		std::cout << "Element " << it->getPort() << " erased" << std::endl; // test: delete later
		it = serverBlocks.erase(it);
	}
	serverBlocks.erase(it, serverBlocks.end());
}


//TODO: Anna is using port as a number, transform this in server, create a function. Not in parsing
//TODO: Move this to config namespace

/*
// ADD DESCRIPTION
void Server::validateAndSetListen(std::vector<std::string> values) {
	std::pair<std::set<std::string>::iterator, bool> ret;

	//std::set<std::string>::iterator it = _listen.begin();

	for (size_t i = 0; i < values.size(); i++) {
		if (!config::_isValidIpPort(values[i])) {
			throw std::runtime_error("Config error: invalid listen values.");
		}
		ret = _listen.insert(values[i]);
		if (!ret.second) {
			throw std::runtime_error("Config error: duplicate value in listen.");
		}
	}

	// delete this, testing
	// for (auto it = _listen.begin(); it != _listen.end(); it++) {
	// 	std::cout << *it << " ";
	// }
	// std::cout << std::endl;
}

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