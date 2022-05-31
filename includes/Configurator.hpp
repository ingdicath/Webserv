//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#pragma once

#include <string>

class Configurator {
public:
	Configurator();

	virtual ~Configurator();

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
	static void setServerDirectives(const std::string &input);
	static void setLocationDirectives(const std::string &input);
};

