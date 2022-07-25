//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include <vector>
#include <string>
#include <set>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stack>
#include "Configurator.hpp"
#include "Server.hpp"
#include "settings.hpp"
#include "utils.hpp"


namespace config {

	Directive splitDirective(std::string &input) {
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
		directive.key = utils::trim(directiveKey);
		directive.value = directiveValues;
		return directive;
	}


	//delete, used for testing purposes
	// void printSet(const std::set<std::string> mySet) {
	// 	std::cout << "my set contains:";
		//	std::set<int>::iterator it;
	// 	for (auto it = mySet.begin(); it != mySet.end(); ++it) {
	// 		std::cout << ' ' << *it;
	// 	}
	// 	std::cout << std::endl;
	// }

	/**
	 * LISTEN
	 */

	// bool _isValidListenValues(const std::string  &values) {
	// 	std::set<std::string> mySet;
	// 	for (size_t i = 0; i < values.size(); i++) {
	// 		if (!_isValidIpPort(values[i])) {
	// 			throw std::runtime_error("Config error: invalid listen values.");
	// 		}
	// 		if (!mySet.insert(values[i]).second) {
	// 			throw std::runtime_error("Config error: duplicate value in listen.");
	// 		}
	// 	}
	// 	return true;
	// }

	//where do i assign correct values for ip and port??
	bool _isValidIpPort(const std::string &listenValue) {
		//	std::string ip;
		//	std::string port;
		size_t numColon = std::count(listenValue.begin(), listenValue.end(), ':');
		size_t posColon = listenValue.find_last_of(':');

		if (numColon > 1 || posColon == (listenValue.size() - 1)) {
			return false;
		} else if (numColon == 1) {
			std::vector<std::string> vec = utils::splitString(listenValue, ':');
			bool valid = _isValidIpv4Address(vec.at(0)) &&
						 _isValidPortRange(vec.at(1));
			return valid;
		} else {
			bool valid = _isValidIpv4Address(listenValue) ||
						 _isValidPortRange(listenValue);
			return valid;
		}
	}

	// Valid ipv4 address 127.127.127.127
	bool _isValidIpv4Address(const std::string &ipAddress) {
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

	bool _isValidPortRange(const std::string &port) {
		size_t portNumber = utils::stringToPositiveNum(port);
		if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
			throw std::runtime_error("Config error: invalid port value: '" + port + "'");
		}
		return true;
	}


/**
 * SERVER NAMES
 */

// check if we can add more regex to validate server names
	bool _isValidServerName(const std::string &serverName) {
		if (serverName[0] == '/' || serverName[0] == '*') {
			throw std::runtime_error("Config error: path can't be a directory or use wildcard *. ");
		}
		if (serverName.find_last_of('/') == serverName.size() - 1 && serverName.size() != 1) {
			throw std::runtime_error("Config error: path can't be a directory.");
		}
		return true;
	}


	bool _isValidServerNames(std::vector<std::string> serverNames) {
		if (serverNames.size() < 1) {
			throw std::runtime_error("Config error: missing argument for server name. ");
		}
		for (size_t i = 0; i < serverNames.size(); i++) {
			if (!&_isValidServerName) {
				throw std::runtime_error("Config error: wrong syntax in server name.");
			}
		}
		return true;
	}


/**
 * ERROR PAGES
 */

	bool _isValidErrorPageConfig(std::vector<std::string> values) {
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
		// printSet(mySet); //delete
		return true;
	}


// check if this validation is necessary
	bool _isValidPath(const std::string &path) {
		if (path[0] != '/') {
			return false;
		}
		if (path.find_last_of('/') == path.size() - 1 && path.size() != 1) {
			throw std::runtime_error("Config error: path can't be a directory.");
		}
		return true;
	}

// max possible error code is 505
	bool _isValidErrorCode(const std::string &string) {
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

	bool _isValidBodySize(std::vector<std::string> values) {
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

	bool _isValidRoot(std::vector<std::string> values) {
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

/**
 * ALLOWED METHODS
 */

	bool _isValidAllowedMethod(std::vector<std::string> values) {
		std::set<std::string> mySet;
		for (size_t i = 0; i < values.size(); i++) {
			values[i] = utils::stringToUpper(values.at(i));
			if (!mySet.insert(values[i]).second) {
				throw std::runtime_error(
						"Config error: duplicate value in allowed_method directive. '" + values[i] + "'");
			}
			if (values[i] != "GET" && values[i] != "POST" && values[i] != "DELETE") {
				throw std::runtime_error(
						"Config error: invalid value in allowed_method directive. '" + values[i] + "'");
			}
		}
		//printSet(mySet); //delete
		return true;
	}

/**
 * INDEX
 * in this case it is allowed just one html page. See if we need to change this in the future
 * to allow more than one page and also other extensions
 */

	bool _isValidIndex(std::vector<std::string> values) {
		if (values.size() != 1) {
			throw std::runtime_error("Config error: invalid args for index.");
		}
		size_t lastPos = values[0].size() - 1;
		if (values[0].find_last_of('/') == lastPos && values[0].size() != 1) {
			throw std::runtime_error("Config error: index can't be a directory.");
		}
		std::string extension = values[0].substr(values[0].find_last_of('.') + 1);
		extension = utils::stringToLower(extension);
		if (extension != "html") {
			throw std::runtime_error("Config error: index can only have html extension");
		}
		return true;
	}


/**
 * AUTOINDEX
 */

	bool _isValidAutoIndex(std::vector<std::string> values) {
		if (values.size() != 1) {
			throw std::runtime_error("Config error: invalid args for autoindex directive.");
		}
		values[0] = utils::stringToLower(values[0]);
		if (values[0] != ON && values[0] != OFF) {
			throw std::runtime_error("Config error: invalid value in autoindex directive. '" + values[0] + "'");
		}
//	if (values[0] == "on") {
//		_autoindex = true;
//	} else if (values[0] == "off") {
//		_autoindex = false;
//	} else {
//		throw std::runtime_error("Config error: invalid value in autoindex directive. '" + values[0] + "'");
//	}
		return true;
	}


/**
 * CGI, We have to manage only one extension. For bonus part, multiple extensions
 */

	bool _isValidCGI(std::vector<std::string> values) {
		if (values.size() != 2) {
			throw std::runtime_error("Config error: invalid args for cgi directive.");
		}

		return true;
	}

	// Translate strings to enums to allow work with switch case
	eDirectives resolveDirective(const std::string &input) {
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

/**
 * CGI, We have to manage only one extension. For bonus part, multiple extensions
 */

	bool in_array(const std::string &value, const std::vector<std::string> &array) {
		return std::find(array.begin(), array.end(), value) != array.end();
	}

	bool isEmptyFile(std::ifstream &inputFile) {
		return inputFile.peek() == std::ifstream::traits_type::eof();
	}

	void openFile(std::ifstream &file, const std::string &filePath) {
		file.open(filePath.c_str(), std::ifstream::in);
		if (!file.is_open()) {
			throw std::runtime_error("Config error: Configuration file failed to open.");
		}
		if (isEmptyFile(file)) {
			throw std::runtime_error("Config error: Empty file.");
		}
//	file.close(); // dont forget to close
	}

	//change name to English
	void validarYAlmacenar(const Directive &directive, std::string &block, std::vector<Server> servers) {
		if (block == "server") {
			almacenarDirectivaEnServer(directive, &servers.back());
		}
	}


//change name to English
	void almacenarDirectivaEnServer(const Directive &directive, Server *server) {
		switch (config::resolveDirective(directive.key)) {
			case config::LISTEN:
				server->validateAndSetListen(directive.value);
				break;
			case config::SERVER_NAME:
				server->validateAndSetServerNames(directive.value);
				break;
//		case config::ERROR_PAGE:
//		server->validateAndSetErrorPages(directive.value);
//			break;
//		case config::BODY_SIZE:
//			//TODO
//			break;
//		case config::ROUTE_LOCATION:
//			//TODO
//			break;
//		case config::ROOT:
//			//TODO
//			break;
//		case config::ACCEPTED_METHODS:
//			//TODO
//			break;
//		case config::INDEX:
//			//TODO
//			break;
//		case config::AUTOINDEX:
//			//TODO
//			break;
//		case config::CGI:
//			//TODO
//			break;
//		case config::UPLOAD:
//			//TODO
//			break;
//		case config::REDIRECTION:
//			//TODO
//			break;
//		case config::INVALID:
//			//TODO, error message
			default:
				break;
		}
	}

	// this function will be inside void    Webserver::loadConfiguration(std::string configFile)
	std::vector<Server> loadConfiguration(const std::string &configFile) {
		std::ifstream file;
		config::openFile(file, configFile); // adjust this to
		std::string line;
		std::vector<Server> servers;
		//Webserver webserver; // destructor debe liberar recursos - delete

		std::string command;
		char c;
		bool comment = false;
		std::stack<std::string> block;

		// loop getting single characters
		while (file.get(c)) {
			switch (c) {
				case '{':
					if (!comment) {
						std::cout << "curly: " << command << std::endl;
						command = utils::trim(command);
						if (command == SERVER) { 					// See if it is better define as enum
							servers.push_back(new Server());		// BE CAREFULL: if you use 'new' you should delete as well
						}
						// TODO extraer location
						size_t postPath = command.find_first_of('/'); //what does happen if there is no '/'??
						std::string cmdLocation = command.substr(0, postPath);
//						std::string locationPath = command.substr(postPath, command.size()-1);

						if (cmdLocation == LOCATION) {
							Server &server = servers.back();
							server.getLocations().push_back(new Location());	// BE CAREFULL: if you use 'new' you should delete as well
						}
						block.push(command); // validar location antes del push
						command = "";
					}
					break;
				case ';':
					if (!comment) {
						std::cout << "linea que se valida: " << command << std::endl;
						Directive directive = config::splitDirective(command);
						config::validarYAlmacenar(directive, block.top(), servers); // WATCH OUT! CHECK THIS
						command = "";
					}
					break;
				case '}':
					if (!comment) {
						if (block.empty() && c == '}') {
							throw std::runtime_error("Config error: unbalanced curly braces.");
						}
						if (c == '}') {
							block.pop();
						}
					}
					break;
				case '#':
					comment = true;
					break;
				case '\n':
					comment = false;
					break;
				default:
					if (!comment)
						command += c;
					break;
			}
		}
		if (!block.empty()) {
			throw std::runtime_error("Config error: unbalanced curly braces.");
		}
		return servers;
	}

} // end of namespace config