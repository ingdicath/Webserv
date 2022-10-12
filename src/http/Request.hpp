//
// Created by Hz Lin on 18/07/2022.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <sstream>

#define CONTENT_LENGTH "Content-Length"
#define HOST "Host"

class Request {
private:
	std::string _method;
	std::string _path;
	std::string _version;
	std::map <std::string, std::string> _headers;
	std::string _host;
	// FOR UPLOAD
	std::string _multipartBoundary;
	std::string _fileName;
	std::string _fileContentType;
	// END UPLOAD
	std::string _body;
	int _ret;

	void parseMethod(std::stringstream &ss);
	void parsePath(std::stringstream &ss);
	void parseVersion(std::stringstream &ss);
	void parseHeaders(std::stringstream &ss);
	void parseBody(std::stringstream &ss, const std::string &requestStr);

	Request();

public:
	Request(const std::string &requesStr);
	virtual ~Request();
	Request(const Request &obj);
	Request &operator=(const Request &obj);

	int parseRawRequest(const std::string &rawRequest);

	//getters
	const std::string &getMethod() const;
	const std::string &getPath() const;
	const std::string &getVersion() const;
	std::map<std::string, std::string> getHeaders() const;
	const std::string &getHost() const;
	const std::string &getBody() const;
	// UPLOAD
	const std::string &getMultipartBoundary() const;
	const std::string &getFileName() const;
	const std::string &getFileContentType() const;
	// END UPLOAD
	const int &getRet() const;
};

// for testing delete later
std::ostream	&operator<<(std::ostream &os, const Request &request);
