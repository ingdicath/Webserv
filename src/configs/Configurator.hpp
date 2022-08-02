//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#pragma once

#include <string>
#include <ostream>
#include <set>
#include "../webserver/Server.hpp"

class Server;

struct Directive {
	std::string key;
	std::vector<std::string> value;
};

namespace config {

	Directive splitDirective(std::string &input);


/** listen */

	bool _isValidPortRange(const std::string &port);

	bool _isValidIpv4Address(const std::string &ipAddress);

	bool _isValidIpPort(const std::string &listenValue);

	bool _isValidListenValues(std::vector<std::string> values);


/** server names */

	bool _isValidServerName(const std::string &serverName);

	bool _isValidServerNames(const std::vector<std::string>& serverNames);


/** error codes and error page */

	bool _isValidErrorPageConfig(std::vector<std::string> values);

//	static bool _isValidPath(const std::string &path);
//
//	static bool _isValidErrorCode(const std::string &string);


/** client max body size */

	//static bool _isValidBodySize(std::vector<std::string> values);

	//static bool _isValidRoot(std::vector<std::string> values);


/** allowed methods */

	//static bool _isValidAllowedMethod(std::vector<std::string> values);


/** index */
	//static bool _isValidIndex(std::vector<std::string> values);


/** autoindex */
	//static bool _isValidAutoIndex(std::vector<std::string> values);

/** cgi */
	bool _isValidCGI(const std::vector<std::string>& values);

	void _checkServerName(const std::string &port);

/** config utils */
	bool isEmptyFile(std::ifstream &inputFile);

	void openFile(std::ifstream &file, const std::string &filePath);

	void validateAndStore(const Directive &directive, std::string &block, std::vector<Server> servers);

	std::vector<Server> loadConfiguration(const std::string &configFile);

	void storeDirectiveInServer(const Directive &directive, Server *server);

	//void loadConfiguration(const std::string &configFile);


	//TODO variables: make private later
	enum eDirectives {
		LISTEN,
		SERVER_NAME,
		ERROR_PAGE,
		BODY_SIZE,
		ROUTE_LOCATION,
		ROOT,
		ACCEPTED_METHODS,
		INDEX,
		AUTOINDEX,
		CGI,
		UPLOAD,
		REDIRECTION,
		INVALID
	};

	enum eBlocks {
		SERVER_,
		LOCATION_
	};

	// std::set<std::string> _listen;
	// bool _autoindex;


	eDirectives resolveDirective(const std::string &input);

	void setServerDirectives(const std::string &rawInput, Server *server);

	//static void setLocationDirectives(const std::string &input);

}
