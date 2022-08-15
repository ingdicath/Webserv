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
 * Load the file and validate it char by char.
 */
std::vector<Server> Parser::validateConfiguration(const std::string &configFile) {
	std::vector<Server> servers;
	std::string line;
	char currentChar;
	bool isComment = false;
	int curlyCounter = 0;
	FileUtils fileUtils;
	fileUtils.openFile(_file, configFile);

	while (_file.get(currentChar)) {
		switch (currentChar) {
			case '{':
				checkOpenCurly(isComment, &curlyCounter, &servers, line);
				std::cout << "Open curly are: " << curlyCounter << std::endl; //test, delete
				line = "";
				break;
			case ';':
				checkSemiColon(isComment, &servers, line);
				line = "";
				break;
			case '}':
				checkCloseCurly(isComment, &curlyCounter, &servers);
				std::cout << "Close curly are: " << curlyCounter << std::endl; //test, delete
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

void Parser::checkOpenCurly(bool isComment, int *curlyCounter, std::vector<Server> *serverBlocks, std::string line) {
	if (!isComment) {
		*curlyCounter += 1;
		if (*curlyCounter > 2) {
			//TODO: create function clean and throw error
			cleanServerBlocks(*serverBlocks);
			throw std::runtime_error("Config error: open unbalanced curly braces.");
		}
		line = utils::trim(line);
		std::cout << "curly: " << line << std::endl;
		// TODO: put extra condition in case key is no server or location
//		if (line != "server" || line != "location") {
//			std::cout << "word different: " << std::endl; //delete thiss
//			cleanServerBlocks(serverBlocks); // check this
//			throw std::runtime_error("No server or location detected."); // doesnt show this exception, check
//		}

		if (line == "server") {                    // See if it is better define as enum
			serverBlocks->push_back(*new Server());        // BE CAREFULL: if you use 'new' you should delete as well
			return;
		}

		// TODO extraer location
		size_t postPath = line.find_first_of('/'); //what does happen if there is no '/'??
		std::string cmdLocation = line.substr(0, postPath);
		std::string pathLocation = line.substr(postPath - 1, line.size());

		if (cmdLocation == "location") {
			Server &server = serverBlocks->back();  //brings last server to create new location
			Location location = new Location(); // BE CAREFULL: if you use 'new' you should delete as well
			location.setPathLocation(pathLocation);
			server.getLocations().push_back(location);
			return;
		}
		//TODO: create function clean and throw error
//		std::cout << "here" << std::endl; //test, delete
//		cleanServerBlocks(*serverBlocks); // check this
//		throw std::runtime_error("No server or location detected."); // update later
	}
}

void Parser::checkSemiColon(bool isComment, std::vector<Server> *serverBlocks, std::string line) {
	if (!isComment) {
		line = utils::trim(line);
		std::cout << "line that is validated: " << line << std::endl; //test, delete
		Directive directive = Parser::splitDirective(line);
		storeDirective(directive, &serverBlocks->back()); // WATCH OUT! CHECK THIS
	}
}

void Parser::checkCloseCurly(bool isComment, int *curlyCounter, std::vector<Server> *serverBlocks) {
	if (!isComment) {
		*curlyCounter -= 1;
		if (*curlyCounter < 0) {
			throw std::runtime_error("Config error: unbalanced closed curly braces.");
		}
	}
}

void Parser::storeDirective(Directive directive, Server *server) {
	int temp;
	switch (Parser::resolveDirective(directive._key)) {
		case PORT:
			temp = validateAndSetPort(directive._value[0]);
			std::cout << "hello port" << temp << std::endl; //test, delete
			server->setPort(temp);
			break;
		case HOST_:
			std::cout << "hello host" << std::endl;
//			server->validateAndSetListen(directive._value);
			break;
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
	bool res = true;
	size_t portNumber = utils::stringToPositiveNum(port);
	if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
		res = false;
	}
	return res;
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

//TODO: complete this function
void Parser::cleanServerBlocks(std::vector<Server> serverBlocks) {
	std::vector<Server>::iterator it = serverBlocks.begin();
	std::cout << "hello it's hot" << std::endl;
	for (; it < serverBlocks.end(); it++) {
		std::cout << "Element " << it->getPort() << " erased" << std::endl; // test: delete later
		it = serverBlocks.erase(it);
	}
//	serverBlocks.erase(it, serverBlocks.end());
}

//TODO: complete this function
void Parser::cleanLocationBlocks(std::vector<Location> locationBlocks) {
	std::vector<Location>::iterator it = locationBlocks.begin();
	for (; it != locationBlocks.end(); it++) {
		it = locationBlocks.erase(it);
	}
	locationBlocks.erase(it, locationBlocks.end());
}

int Parser::validateAndSetPort(const std::string &port) {
	if (!Parser::_isValidPortRange(port)) {
		throw InvalidPortRangeException();
		throw std::invalid_argument("Config error: invalid port value: '" + port + "'");
	}
	return utils::strToInt(port);
}







//TODO: Anna is using port as a number, transform this in server, create a function. Not in parsing
//TODO: Move this to config namespace

/*
// ADD DESCRIPTION
void Server::validateAndSetPort(std::vector<std::string> values) {
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