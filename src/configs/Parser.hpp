//
// Created by Diani on 08/08/2022.
//

#pragma once

#include <fstream>
#include "../webserver/Server.hpp"

class Parser {
public:
	Parser();

	virtual ~Parser();

	std::vector<Server> validateConfiguration(const std::string &configFile);

	void cleanServerBlocks(std::vector<Server> *);

	void cleanLocationBlocks(std::vector<Location> *);


	class ConfigFileException : public std::exception {
	public:
		explicit ConfigFileException(const std::string &message);

		~ConfigFileException() throw();

		virtual const char *what() const throw();

	private:
		std::string msg_;
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

	void _checkSemiColon(bool isComment, std::vector<Server> *serverBlocks, std::string line);

	void _checkCloseCurly(bool isComment, std::stack<std::string> *sectionBlock);

	void _checkOpenCurly(bool isComment, std::stack<std::string> *sectionBlock, std::vector<Server> *serverBlocks,
						 std::string line);

	void _storeDirective(Directive directive, Server *server);

	eDirectives _resolveDirective(const std::string &input);

	Directive _splitDirective(std::string &input);


	/** Functions to check parameters inside config file **/

	bool _isValidPortRange(const std::string &port);

	bool _isValidIpv4Address(const std::string &ipAddress);

	bool _isValidServerName(std::string serverName);

	bool _isValidServerNames(const std::vector<std::string> &serverNames);

	int _checkPort(const std::string &port);

	std::string _checkHost(std::string host);

	std::vector<std::string> _checkServerNames(std::vector<std::string> serverNames);

};
