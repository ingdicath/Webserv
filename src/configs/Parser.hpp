//
// Created by Diani on 08/08/2022.
//

#pragma once

#include <fstream>
#include "../webserver/Server.hpp"

class Parser {
public:

	std::vector<Server> validateConfiguration(const std::string &configFile);

	void cleanServerBlocks(std::vector<Server>);

	class InvalidPortRangeException : public std::exception {
	public:
		const char *what() const throw() {
			return "Config error: invalid port value: '";
		}
	};

private:
	std::ifstream _file;

	struct Directive {
		std::string _key;
		std::vector<std::string> _value;
	};

	enum eDirectives {
		PORT,
		HOST_,  //conflict with macro's Request.hpp
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

	void checkSemiColon(std::string line, bool isComment, std::vector<Server> serverBlocks);

	void checkCloseCurly(bool isComment, int *pInt, std::vector<Server> serverBlocks);

	void checkOpenCurly(std::vector<Server> serverBlocks, bool isComment, std::string line, int *curlyCounter);

	void storeDirective(Directive directive, Server *server);

	eDirectives resolveDirective(const std::string &input);

	Directive splitDirective(std::string &input);

	bool _isValidPortRange(const std::string &port);

	bool _isValidIpv4Address(const std::string &ipAddress);

	bool _isValidServerName(const std::string &serverName);
};



