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
 * The configuration file is being read char by char.
 * @param configFile is the path where is located the configuration file.
 */

void Parser::validateConfiguration(const std::string &configFile, std::vector<Server> *servers) {
	std::string line;
	char currentChar;
	bool isComment = false;
	std::stack<std::string> sectionBlock; //to validate server and location keyword
	FileUtils fileUtils;
	fileUtils.openFile(_file, configFile);

	while (_file.get(currentChar)) {
		switch (currentChar) {
			case '{':
				_checkOpenCurly(isComment, &sectionBlock, servers, line);
				line = "";
				break;
			case ';':
				_checkSemiColon(isComment, servers, line, sectionBlock);
				line = "";
				break;
			case '}':
				_checkCloseCurly(isComment, &sectionBlock, line);
				break;
			case '#':
				isComment = true;
				break;
			case '\n':
				isComment = false;
				break;
			default:
				if (!isComment) {
					line += currentChar;
				}
				break;
		}
	}
	if (!sectionBlock.empty()) {
		throw ConfigFileException("Open unbalanced curly braces.");
	} else if (sectionBlock.empty() && !utils::trim(line).empty()) {
		throw ConfigFileException("Found invalid characters outside blocks: '" + utils::trim(line) + "'.");
	} else if (!_isValidPortServerNameDupla(servers)) {
		throw ConfigFileException("Found same port(s) with same server name(s).");
	}
	fileUtils.closeFile(_file);
}

/**
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
		int posPath = static_cast<int>(line.find_first_of('/'));
		std::string command = line.substr(0, posPath);
		command = utils::trim(command);
		if (sectionBlock->empty() && line == "server") {
			Server *temp = new Server(); // memory management
			serverBlocks->push_back(*temp);
			delete temp;
		} else if (sectionBlock->empty() && command == "location") {
			throw ConfigFileException("location block found outside server block.");
		} else if (sectionBlock->empty() && command != "location" && command != "server") {
			throw ConfigFileException("Unbalanced/wrong configuration in: '" + command +
									  "'. Keywords allowed in this section are 'server' or 'location'.");
		} else if (sectionBlock->top() == "server" && command == "location" && posPath != -1) {
			Server &server = serverBlocks->back();  //brings last server to create new location
			Location *l = _checkLocation(line, posPath - 1);
			server.addLocation(*l);
			delete l;
		} else if (sectionBlock->top() == "location" && command == "location") {
			throw ConfigFileException("Location block unclosed -- Nested location not allowed.");
		} else {
			throw ConfigFileException("Invalid value, no server or location detected in open block.");
		}
		sectionBlock->push(command);
	}
}

/**
 * When the line reaches a semicolon (;), it is stored in a directive data structure
 * composed by two parts: key and value(e.g. key-> port, value-> 80).
 * Then, this directive is stored in the last server added in the vector of servers.
 */
void Parser::_checkSemiColon(bool isComment, std::vector<Server> *serverBlocks, std::string line,
							 std::stack<std::string> section) {
	if (!isComment) {
		line = utils::trim(line);
		Directive directive = Parser::_splitDirective(line);
		if (section.empty()) {
			throw ConfigFileException("Found invalid characters ended with semicolon outside blocks.");
		}
		if (section.top() == "server")
			_storeServerDirective(directive, &serverBlocks->back());
		else if ((section.top() == "location"))
			_storeLocationDirective(directive, &serverBlocks->back().getLocations()->back());
	}
}

/**
 * When the line reaches a close curly brace, it is verified if there is a server
 * or location block opened.
 */
void Parser::_checkCloseCurly(bool isComment, std::stack<std::string> *sectionBlock, std::string line) {
	if (!isComment) {
		line = utils::trim(line);
		if (sectionBlock->empty()) {
			throw ConfigFileException("Unbalanced curly braces.");
		} else if (!line.empty()) {
			throw ConfigFileException("Wrong syntax in line: '" + utils::trim(line) + "'.");
		}
		sectionBlock->pop();
	}
}

void Parser::_storeServerDirective(const Directive &directive, Server *server) {
	switch (Parser::_resolveDirective(directive._key)) {
		case PORT:
			server->setPort(_checkPort(directive._value));
			break;
		case HOST_:
			server->setHost(_checkHost(directive._value));
			break;
		case SERVER_NAME:
			server->setServerName(_checkServerNames(directive._value));
			break;
		case ERROR_PAGE:
			server->addErrorPage(_checkErrorPage(directive._value));
			break;
		case BODY_SIZE:
			server->setClientMaxBodySize(_checkBodySize(directive._value));
			break;
		case INVALID:
			std::cerr << RED ERROR " Invalid directive: '" + directive._key + "'." RESET
					  << std::endl;
			throw ConfigFileException("Wrong argument for directive.");
		default:
			throw ConfigFileException("'" + directive._key + "' is not allowed in server block.");
	}
}

void Parser::_storeLocationDirective(const Parser::Directive &directive, Location *location) {
	switch (Parser::_resolveDirective(directive._key)) {
		case INDEX:
			location->setIndex(_checkIndex(directive._value));
			break;
		case ROOT:
			location->setRoot(_checkRoot(directive._value));
			break;
		case ACCEPTED_METHODS:
			location->setMethods(_checkAcceptedMethods(directive._value));
			break;
		case AUTOINDEX:
			location->setAutoindex(_checkAutoindex(directive._value)); //retrieves the last location
			break;
		case CGI:
			location->setCGI(_checkCGI(directive._value));
			break;
		case UPLOAD:
			location->setUpload(_checkUpload(directive._value));
			break;
		case REDIRECTION:
			location->addRedirection(_checkRedirection(directive._value));
			break;
		case INVALID:
			std::cerr << RED ERROR " Invalid directive: '" + directive._key + "'." RESET << std::endl;
			throw ConfigFileException("Wrong argument for directive.");
		default:
			throw ConfigFileException("'" + directive._key + "' is not allowed in location block.");
	}
}


/**
 * Translate strings to enums to allow work with switch case.
 */
Parser::eDirectives Parser::_resolveDirective(const std::string &input) {
	if (input == "port") return Parser::PORT;
	if (input == "host") return Parser::HOST_;
	if (input == "server_name") return Parser::SERVER_NAME;
	if (input == "error_page") return Parser::ERROR_PAGE;
	if (input == "client_max_body_size") return Parser::BODY_SIZE;
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
 * @param input Refers to the line that contains keyword (e.g. port) and
 * its value (e.g. 80)
 */
Parser::Directive Parser::_splitDirective(std::string &input) {
	std::string cleanInput = utils::trim(input);
	size_t splitPos = cleanInput.find_first_of(WHITESPACES);

	// check if key and value exists
	if (splitPos > input.size()) {
		throw ConfigFileException("Unbalanced directive key: '" + cleanInput + "'.");
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


/************************************************************************************
* 				Functions that verify if a parameter is valid or not				*
************************************************************************************/

bool Parser::_isValidPortRange(const std::string &port) {
	size_t portNumber = utils::stringToPositiveNum(port);
	if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
		return false;
	}
	return true;
}

/**
 * A valid value for port can be 'localhost' or an IPv4 address like '127.0.0.0'
 */
bool Parser::_isValidIpv4Address(const std::string &ipAddress) {
	if (ipAddress == "localhost") {
		return true;
	}
	size_t numDots = std::count(ipAddress.begin(), ipAddress.end(), '.');
	std::vector<std::string> vec = utils::splitString(ipAddress, '.');
	if (numDots > 3 || vec.size() != 4) {
		std::cerr << RED ERROR " Wrong syntax for 'host': '" + ipAddress + "'" RESET << std::endl;
		return false;
	}
	size_t num;
	for (size_t i = 0; i < vec.size(); i++) {
		num = utils::stringToPositiveNum(vec.at(i));
		if (num > 255) {
			std::cerr << RED ERROR "Wrong values for 'host': '" + ipAddress + "'" RESET << std::endl;
			return false;
		}
	}
	return true;
}

bool Parser::_isValidServerName(std::string serverName) {
	serverName = utils::trim(serverName);
	if (serverName[0] == '/' || serverName[0] == '*') {
		std::cerr << RED ERROR " 'server_name' can't be a directory or use wildcard *. " RESET << std::endl;
		return false;
	}
	if (serverName.find_last_of('/') == serverName.size() - 1 && serverName.size() != 1) {
		std::cerr << RED ERROR " 'server_name' can't be a directory." RESET << std::endl;
		return false;
	}
	return true;
}

/**
 * @param serverNames vector of server names to be evaluated.
 */
bool Parser::_areValidServerNames(const std::vector<std::string> &serverNames) {
	for (size_t i = 0; i < serverNames.size(); i++) {
		if (!_isValidServerName(serverNames.at(i))) {
			return false;
		}
	}
	return true;
}

/**
 * Max possible error status code is 505 and for redirection is 307.
 * @param statusCode is the status code to validate.
 */
bool Parser::_isValidStatusCode(const std::string &statusCode, const std::string &directive) {
	if (!utils::isPositiveNumber(statusCode)) {
		std::cerr << RED ERROR " Status code must be a positive value: '"
					 + statusCode + "'." RESET << std::endl;
		return false;
	}
	size_t num = utils::stringToPositiveNum(statusCode);
	if (directive == "error_page") {
		if (num < 400 || num > 505) {
			std::cerr << RED ERROR " Status code must be between 400 and 506: '"
						 + statusCode + "'" RESET << std::endl;
			return false;
		}
	} else if (directive == "redirection") {
		if (num < 300 || num > 307) {
			std::cerr << RED ERROR " Status code must be between 300 and 307: '"
						 + statusCode + "'" RESET << std::endl;
			return false;
		}
	}
	return true;
}


bool Parser::_isValidPath(const std::string &path, const std::string &directive) {
	if (path.find_last_of('/') == path.size() - 1 && path.size() != 1 && directive != "location") {
		std::cerr << RED ERROR " Invalid syntax for '" + directive +
					 "', remove the last '/': '" + path + "'." RESET << std::endl;
		return false;
	}
	return true;
}

bool Parser::_isValidPathLocation(std::string path, const std::string &directive) {
	if (path[0] != '/') {
		std::cerr << RED ERROR " Path for '" + directive + "' must be start with '/': '"
					 + path + "'." RESET << std::endl;
		return false;
	}
	return true;
}

/**
 * @param values represents the error code (at vector position 0)
 * and the url path (at vector position 1).
 */
bool Parser::_isValidErrorPage(std::vector<std::string> values) {
	if (values.size() != 2 || !_isValidStatusCode(values[0], "error_page")
		|| !_isValidPath(values[1], "error_page")) {
		return false;
	}
	return true;
}

/**
 * In this case it is allowed just one html page. See if we need to change this in the future
 * to allow more than one page and also other extensions
 */
bool Parser::_isValidIndex(const std::string &index) {
	size_t lastPos = index.size() - 1;
	if (index.find_last_of('/') == lastPos && index.size() != 1) {
		std::cerr << RED ERROR " 'index' can't be a directory: '" + index
					 + "'." RESET << std::endl;
		return false;
	}
	return true;
}

bool Parser::_isValidBodySize(std::string bodySize) {
	size_t lastPos = bodySize.size() - 1;
	if (bodySize.find_first_of("Mm") != lastPos) {
		std::cerr << RED ERROR " Only megabytes (M or m) measure is allowed "
					 "for 'body_size' directive: '" + bodySize + "'" RESET << std::endl;
		return false;
	}
	std::string bodyNumber = bodySize.substr(bodySize.find_first_of("Mm") + 1);
	bodySize.at(lastPos) =
			char(std::toupper(static_cast<unsigned char>(bodySize.at(lastPos))));
	bodyNumber = utils::deleteLastOf('M', bodySize);
	if (!utils::isPositiveNumber(bodyNumber)) {
		std::cerr << RED ERROR " Size must be a positive number followed by 'M' (e.g 100M): '"
					 + bodySize + "'" RESET << std::endl;
		return false;
	}
	return true;
}

bool Parser::_isValidAcceptedMethod(std::vector<std::string> values) {
	std::string toUpper;
	for (size_t i = 0; i < values.size(); i++) {
		toUpper = utils::stringToUpper(values.at(i));
		if (toUpper != "GET" && toUpper != "POST" && toUpper != "DELETE") {
			std::cerr << RED ERROR " Invalid argument for 'accepted_methods': '"
						 + values[i] + "'." RESET << std::endl;
			return false;
		}
	}
	return true;
}

bool Parser::_isValidAutoindex(const std::string &autoindex) {
	std::string toLower;
	toLower = utils::stringToLower(autoindex);
	if (toLower != "on" && toLower != "off") {
		std::cerr << RED ERROR " Value doesn't match with 'on' or 'off' required for 'autoindex': '"
					 + autoindex + "'." RESET << std::endl;
		return false;
	}
	return true;
}

bool Parser::_isValidRedirection(std::vector<std::string> values) {
	if (values.size() < 2 || !_isValidStatusCode(values[0], "redirection")) {
		return false;
	}
	if (values[1].find_last_of('/') == values[1].size() - 1 && values[1].size() != 1) {
		std::cerr << RED ERROR " Path for 'redirection' can't be a directory: '"
					 + values[1] + "'." RESET << std::endl;
		return false;
	}
	return true;
}

bool Parser::_isValidExtension(const std::string &extension) {
	size_t start = extension.find_first_of('.');
	if (start != 0) {
		return false;
	}
	size_t numDots = std::count(extension.begin(), extension.end(), '.');
	if (numDots != 1) {
		return false;
	}
	return true;
}

bool Parser::_isValidCGI(const std::vector<std::string> &cgi) {
	if (cgi.size() != 2) {
		return false;
	}
	if (!_isValidExtension(cgi[0])) {
		std::cerr << RED ERROR " Invalid extension for 'cgi': '" + cgi[0]
					 + "'." RESET << std::endl;
		return false;
	}
	if (!_isValidPath(cgi[1], "cgi")) {
		return false;
	}
	return true;
}

/************************************************************************************
* 							Functions to check parameters							*
************************************************************************************/

int Parser::_checkPort(std::vector<std::string> port) {
	if (port.size() > 1) {
		throw ConfigFileException("Invalid number of arguments for 'port'. "
								  "Only one argument is allowed followed by ';'.");
	}
	if (!Parser::_isValidPortRange(port[0])) {
		throw ConfigFileException("Invalid value for 'port'.");
	}
	return utils::stringToInt(port[0]);
}

std::string Parser::_checkHost(std::vector<std::string> host) {
	if (host.size() > 1) {
		throw ConfigFileException("Invalid number of arguments for 'host'. "
								  "Only one argument is allowed followed by ';'.");
	}
	if (!Parser::_isValidIpv4Address(host[0])) {
		throw ConfigFileException("Invalid value for 'host'.");
	}
	if (host[0] == "localhost") {
		host[0] = "127.0.0.1";
	}
	return host[0];
}

std::vector<std::string> Parser::_checkServerNames(std::vector<std::string> serverNames) {
	if (!_areValidServerNames(serverNames)) {
		throw ConfigFileException("Invalid value(s) for 'server_name'.");
	}
	std::vector<std::string> myVector;
	for (size_t i = 0; i < serverNames.size(); i++) {
		myVector.push_back(serverNames[i]);
	}
	return myVector;
}

std::pair<int, std::string> Parser::_checkErrorPage(std::vector<std::string> errorPage) {
	if (errorPage.size() > 2) {
		throw ConfigFileException("Invalid number of arguments for 'error_page'. "
								  "Two arguments are expected followed by ';'.");
	}
	if (!_isValidErrorPage(errorPage)) {
		throw ConfigFileException("Invalid arguments for 'error_page'.");
	}
	std::pair<int, std::string> res;
	res = std::make_pair(utils::stringToInt(errorPage[0]), errorPage[1]);
	return res;
}

std::string Parser::_checkIndex(std::vector<std::string> index) {
	if (index.size() > 1) {
		throw ConfigFileException("Invalid number of arguments for 'index'. "
								  "Only one argument is allowed.");
	}
	if (!_isValidIndex(index[0])) {
		throw ConfigFileException("Invalid argument for 'index'.");
	}
	return index[0];
}

long Parser::_checkBodySize(std::vector<std::string> bodySize) {
	if (bodySize.size() != 1) {
		throw ConfigFileException("Only one argument is allowed for 'client_max_body_size'.");
	}
	if (!_isValidBodySize(bodySize[0])) {
		throw ConfigFileException("Invalid argument for 'client_max_body_size'.");
	}
	size_t lastPos = bodySize.size() - 1;
	bodySize[0].at(lastPos) =
			char(std::toupper(static_cast<unsigned char>(bodySize[0].at(lastPos))));
	bodySize[0] = utils::deleteLastOf('M', bodySize[0]);
	long res = utils::stringToLong(bodySize[0]);
	return res;
}

Location *Parser::_checkLocation(const std::string &line, const int &posPath) {
	std::string pathLocation = line.substr(posPath, line.size());
	pathLocation = utils::trim(pathLocation);
	std::vector<std::string> myVec = utils::splitByWhiteSpaces(pathLocation, WHITESPACES);
	if (myVec.size() != 1 || !_isValidPathLocation(pathLocation, "location")) {
		throw ConfigFileException("Invalid arguments for path in Location block.");
	}
	if (pathLocation.find_last_of('/') != pathLocation.size() - 1 && pathLocation.size() != 1) {
		throw ConfigFileException("Path location '" + pathLocation + "' must end with '/'.");
	}
	return &(*new Location(pathLocation));
}

std::set<std::string> Parser::_checkAcceptedMethods(std::vector<std::string> methods) {
	if (!_isValidAcceptedMethod(methods)) {
		throw ConfigFileException("Invalid arguments for 'accepted_methods'.");
	}
	std::set<std::string> mySet;
	for (size_t i = 0; i < methods.size(); i++) {
		methods[i] = utils::stringToUpper(methods.at(i));
		if (!mySet.insert(methods[i]).second) {
			throw ConfigFileException(
					" Duplicate value in 'accepted_methods'. '" + methods[i] + "'");
		}
		if (methods[i] != "GET" && methods[i] != "POST" && methods[i] != "DELETE") {
			throw ConfigFileException(
					" Invalid value for 'accepted_methods'. '" + methods[i] + "'");
		}
	}
	return mySet;
}

bool Parser::_checkAutoindex(std::vector<std::string> autoIndex) {
	if (autoIndex.size() > 1) {
		throw ConfigFileException("Only one argument is allowed for 'autoindex'.");
	}
	if (!_isValidAutoindex(autoIndex[0])) {
		throw ConfigFileException("Invalid value for 'autoindex'.");
	}
	if (autoIndex[0] == "on") {
		return true;
	} else {
		return false;
	}
}

std::string Parser::_checkRoot(std::vector<std::string> root) {
	if (root.size() > 1) {
		throw ConfigFileException("Only one argument is allowed for 'root'.");
	}
	if (!_isValidPath(root[0], "root")) {
		throw ConfigFileException("Invalid value for 'root'.");
	}
	if (root[0][0] == '/') {
		root[0].erase(root[0].begin());
	}
	char realPath[MAXLINE];
	realpath(root[0].c_str(), realPath);
	root[0] = realPath;
	return root[0];
}

std::pair<int, std::string> Parser::_checkRedirection(std::vector<std::string> redir) {
	if (redir.size() > 2) {
		throw ConfigFileException("Invalid number of arguments for 'redirection'. "
								  "Two arguments are expected followed by ';'.");
	}
	if (!_isValidRedirection(redir)) {
		throw ConfigFileException("Invalid value(s) for 'redirection'.");
	}
	std::pair<int, std::string> res;
	res = std::make_pair(utils::stringToInt(redir[0]), redir[1]);
	return res;
}

std::pair<std::string, std::string> Parser::_checkCGI(std::vector<std::string> cgi) {
	if (cgi.size() > 2) {
		throw ConfigFileException("Invalid number of arguments for 'cgi'. "
								  "Two arguments are expected followed by ';'.");
	}
	if (!_isValidCGI(cgi)) {
		throw ConfigFileException("Invalid value(s) for 'cgi'.");
	}
	std::pair<std::string, std::string> myPair;
	myPair = std::make_pair(cgi[0], cgi[1]);
	return myPair;
}

std::string Parser::_checkUpload(std::vector<std::string> upload) {
	if (upload.size() > 1) {
		throw ConfigFileException("Only one argument is allowed for 'upload'.");
	}
	if (!_isValidPath(upload[0], "upload")) {
		throw ConfigFileException("Invalid value(s) for 'upload'.");
	}
	if (upload[0][0] != '/') {
		upload[0] = "/" + upload[0];
	}
	return upload[0];
}

void Parser::setDefaultServer(std::vector<Server> *serverBlocks) {
	std::vector<Server>::iterator itEval = serverBlocks->begin();
	std::vector<Server>::iterator itCurrent = serverBlocks->begin();
	if (!serverBlocks->empty()) {
		itEval->setIsDefault(true);
	}
	for (itEval++; itEval != serverBlocks->end(); itEval++) {
		for (itCurrent = serverBlocks->begin(); itCurrent != itEval; itCurrent++) {
			if (itEval->getPort() == itCurrent->getPort()) {
				break;
			}
		}
		if (itEval == itCurrent) {
			itEval->setIsDefault(true);
		}
	}
}

bool Parser::_isValidPortServerNameDupla(std::vector<Server> *serverBlocks) {
	std::vector<Server>::iterator itEval = serverBlocks->begin();
	std::vector<Server>::iterator itCurrent = serverBlocks->begin();
	std::vector<std::string> res;
	for (itEval++; itEval != serverBlocks->end(); itEval++) {
		for (itCurrent = serverBlocks->begin(); itCurrent != itEval; itCurrent++) {
			if (itEval->getPort() == itCurrent->getPort()) {
				std::vector<std::string> inter = _findIntersection(itEval->getServerName(), itCurrent->getServerName());
				if (!inter.empty()) {
					std::cerr << RED ERROR " Conflicting server_name '"
							  << inter[0] //TODO: print all the intersection
							  << "' on port: '" << itEval->getPort() << "'." RESET << std::endl;
					return false;
				}
				break;
			}
		}
	}
	return true;
}

std::vector<std::string> Parser::_findIntersection(std::vector<std::string> v1, std::vector<std::string> v2) {
	std::set<std::string> s1(v1.begin(), v1.end());
	std::set<std::string> s2(v2.begin(), v2.end());
	std::vector<std::string> v3;
	std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(v3));
//	if (!v3.empty()) {
//		for (std::vector<std::string>::iterator it = v3.begin(); it != v3.end(); it++)
//			std::cout << *it << ' ';
//	}
	return v3;
}
void Parser::_printVector(std::vector<std::string> input) {
	if (!input.empty()) {
		for (std::vector<std::string>::iterator it = input.begin(); it != input.end(); it++)
			std::cout << *it << ' ';
	}
}


/************************************************************************************
* 										Exception									*
************************************************************************************/

/**
 * This is a custom exception to show error messages when the configuration file is processed.
 */
Parser::ConfigFileException::ConfigFileException(const std::string &message)
		: _msg("[ERROR] " + message) {}

Parser::ConfigFileException::~ConfigFileException() throw() {}

const char *Parser::ConfigFileException::what() const throw() {
	return _msg.c_str();
}
