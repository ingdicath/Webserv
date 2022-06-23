//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#pragma once

#include <string>
#include <ostream>
#include <set>
#include "Server.hpp"

struct Directive {
	std::string key;
	std::vector<std::string> value;
};

class Configurator {
public:
	Configurator();

	virtual ~Configurator();

	static Directive splitDirective(std::string &input);


/** listen */

	static bool _isValidPortRange(const std::string &port);

	static bool _isValidIpv4Address(const std::string &ipAddress);

	static bool _isValidIpPort(const std::string &listenValue);

	static bool _isValidListenValues(std::vector<std::string> values);


/** error codes and error page */

	static bool _isValidErrorPageConfig(std::vector<std::string> values);

	static bool _isValidPath(const std::string &path);

	static bool _isValidErrorCode(const std::string &string);


/** client max body size */

	static bool _isValidBodySize(std::vector<std::string> values);

	static bool _isValidRoot(std::vector<std::string> values);


/** allowed methods */

	static bool _isValidAllowedMethod(std::vector<std::string> values);


/** index */
	static bool _isValidIndex(std::vector<std::string> values);


/** autoindex */
	static bool _isValidAutoIndex(std::vector<std::string> values);

/** cgi */
	bool _isValidCGI(std::vector<std::string> values);

	void _checkServerName(const std::string &port);

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

	std::set<std::string> _listen;
	bool _autoindex;


	static eDirectives resolveDirective(const std::string &input);

	void setServerDirectives(const std::string &rawInput, Server *server);

	static void setLocationDirectives(const std::string &input);

};

