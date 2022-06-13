//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#pragma once

#include <string>
#include "Server.hpp"

class Configurator {
public:
	Configurator();

	virtual ~Configurator();

	static std::pair<std::string, std::string> splitDirective(std::string &input); // become private later


	static bool _isValidPortRange(const std::string& port);
	static bool _isValidIpv4Address(const std::string &ipAddress);
	static bool _isValidIpPort(const std::string &listenValue);

	static bool _isValidPath(const std::string &path);
	static bool _isValidErrorCode(const std::string &string);
	static bool _isValidErrorPageConfig(std::string &string);
	static bool _isValidBodySize(std::string &string);
	void _checkServerName(const std::string& port);

private:
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

	static eDirectives resolveDirective(const std::string &input);


	void setServerDirectives(const std::string &rawInput, Server *server);

	static void setLocationDirectives(const std::string &input);


};

