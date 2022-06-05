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

