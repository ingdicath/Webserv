//
// Created by Diani on 08/08/2022.
//

#pragma once

#include <fstream>
#include <stack>
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
		std::string _msg;
	};

private:
	std::ifstream _file;
	std::set<std::string> _uniqueDirectives;

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
//		ROUTE_LOCATION,
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
	void _storeDirective(const Directive &directive, Server *server);
	eDirectives _resolveDirective(const std::string &input);
	Directive _splitDirective(std::string &input);
	void _addDirectiveToSet(std::string directive);


	/** Functions to check parameters inside config file **/

	bool _isValidPortRange(const std::string &port);
	bool _isValidIpv4Address(const std::string &ipAddress);
	bool _isValidServerName(std::string serverName);
	bool _areValidServerNames(const std::vector<std::string> &serverNames);
	bool _isValidStatusCode(const std::string &statusCode, const std::string &directive);
	bool _isValidPath(std::string path, const std::string &directive);
	bool _isValidErrorPage(std::vector<std::string> values);
	bool _isValidIndex(const std::string &index);
	bool _isValidBodySize(std::string bodySize);
	bool _isValidAcceptedMethod(std::vector<std::string> values);
	bool _isValidAutoindex(const std::string &autoindex);
	bool _isValidRoot(const std::string &root);
	bool _isValidExtension(const std::string &extension);
	bool _isValidPathLocation(std::string pathLoc, const std::string &directive);

	int _checkPort(std::vector<std::string> port);
	std::string _checkHost(std::vector<std::string> host);
	std::vector<std::string> _checkServerNames(std::vector<std::string> serverNames);
	std::map<int, std::string> _checkErrorPage(std::vector<std::string> errorPage);
	std::string _checkIndex(std::vector<std::string> index);
	long _checkBodySize(std::vector<std::string> bodySize);
	bool _checkAutoindex(std::vector<std::string> autoIndex);
	std::set<std::string> _checkAcceptedMethods(std::vector<std::string> methods);
	std::string _checkRoot(std::vector<std::string> root);
	std::map<int, std::string> _checkRedirection(std::vector<std::string> redir);
	bool _isValidRedirection(std::vector<std::string> redir);
	std::pair<std::string, std::string> _checkCGI(std::vector<std::string> cgi);
	bool _isValidCGI(const std::vector<std::string> &cgi);
	std::string _checkUpload(std::vector<std::string> upload);
//	std::string _checkpathLocation(std::vector<std::string> pathLoc);
};
