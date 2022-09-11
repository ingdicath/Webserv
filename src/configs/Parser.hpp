//
// Created by Diani on 08/08/2022.
//

#pragma once

#include <fstream>
#include <stack>
#include "../webserver/Server.hpp"
#include "../webserver/Webserver.hpp"

class Parser {
public:
	Parser();
	virtual ~Parser();
	void validateConfiguration(const std::string &configFile, std::vector<Server> *servers);

	class ConfigFileException : public std::exception {
	public:
		explicit ConfigFileException(const std::string &message);
		~ConfigFileException() throw();
		virtual const char *what() const throw();
	private:
		std::string _msg;
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
		ROOT,
		ACCEPTED_METHODS,
		INDEX,
		AUTOINDEX,
		CGI,
		UPLOAD,
		REDIRECTION,
		INVALID
	};

	void _checkSemiColon(bool isComment, std::vector<Server> *serverBlocks,
						 std::string line, std::stack<std::string> section);
	void _checkOpenCurly(bool isComment, std::stack<std::string> *sectionBlock,
						 std::vector<Server> *serverBlocks, std::string line);
	void _checkCloseCurly(bool isComment, std::stack<std::string> *sectionBlock,
						  std::string line);
	void _storeServerDirective(const Directive &directive, Server *server);
	void _storeLocationDirective(const Directive &directive, Location *location);
	eDirectives _resolveDirective(const std::string &input);
	Directive _splitDirective(std::string &input);


	/** Functions to check parameters inside config file **/

	bool _isValidPortRange(const std::string &port);
	bool _isValidIpv4Address(const std::string &ipAddress);
	bool _isValidServerName(std::string serverName);
	bool _areValidServerNames(const std::vector<std::string> &serverNames);
	bool _isValidStatusCode(const std::string &statusCode, const std::string &directive);
	bool _isValidPath(std::string path, const std::string &directive);
	bool _isValidPathLocation(std::string path, const std::string &directive);
	bool _isValidErrorPage(std::vector<std::string> values);
	bool _isValidIndex(const std::string &index);
	bool _isValidBodySize(std::string bodySize);
	bool _isValidAcceptedMethod(std::vector<std::string> values);
	bool _isValidAutoindex(const std::string &autoindex);
	bool _isValidExtension(const std::string &extension);
	bool _isValidRedirection(std::vector<std::string> redir);
	bool _isValidCGI(const std::vector<std::string> &cgi);

	int _checkPort(std::vector<std::string> port);
	long _checkBodySize(std::vector<std::string> bodySize);
	bool _checkAutoindex(std::vector<std::string> autoIndex);
	std::string _checkHost(std::vector<std::string> host);
	std::string _checkUpload(std::vector<std::string> upload);
	std::string _checkIndex(std::vector<std::string> index);
	std::string _checkRoot(std::vector<std::string> root);
	std::vector<std::string> _checkServerNames(std::vector<std::string> serverNames);
	std::pair<int, std::string> _checkErrorPage(std::vector<std::string> errorPage);
	std::pair<int, std::string> _checkRedirection(std::vector<std::string> redir);
	std::pair<std::string, std::string> _checkCGI(std::vector<std::string> cgi);
	std::set<std::string> _checkAcceptedMethods(std::vector<std::string> methods);
	Location *_checkLocation(const std::string &line, const int &posPath);
};
