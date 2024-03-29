/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: hlin <hlin@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 10:35:35 by hlin          #+#    #+#                 */
/*   Updated: 2022/10/25 10:35:35 by hlin          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iomanip>
#include "Request.hpp"
#include "../utils/settings.hpp"
#include "../utils/utils.hpp"

Request::Request() : _ret(200){
}

Request::Request(const std::string &requestStr) :
	_body(""), _ret(200) {
	parseRawRequest(requestStr);
	if (_ret != 200) {
		std::cerr << RED "Request parsing error: " << _ret << RESET << std::endl;
	}
}

Request::Request(const Request &obj) {
	*this = obj;
}

Request &Request::operator=(const Request &obj) {
	if (this != &obj) {
		_method = obj._method;
		_path = obj._path;
		_version = obj._version;
		_headers = obj._headers;
		_body = obj._body;
		_host = obj._host;
	}
	return *this;
}

Request::~Request() {
}

// Getters
const std::string  &Request::getMethod() const {
	return _method;
}

const std::string    &Request::getPath() const {
	return _path;
}

std::map<std::string, std::string>	Request::getHeaders() const {
	return _headers;
}

const std::string &Request::getHost() const {
	return _host;
}

const std::string &Request::getBody() const {
	return _body;
}

const int &Request::getRet() const {
	return _ret;
}

const std::string &Request::getFileName() const {
	return _fileName;
}

//setters
void Request::setRet(int ret) {
	_ret = ret;
}

int Request::parseRawRequest(const std::string &rawRequest) {
if (rawRequest.find("\r\n\r\n") != std::string::npos) {
	std::stringstream ss(rawRequest);
	parseMethod(ss);
	parsePath(ss);
	parseVersion(ss);
	ss.ignore(2); // skip the \r\n
	parseHeaders(ss);
	parseBody(ss, rawRequest);
	} else {
		_ret = 400;
	}
	return _ret;
}

void    Request::parseMethod(std::stringstream &ss) {
	ss >> _method;
	if (_method == "GET" || _method == "POST" || _method == "DELETE") {
		return;
	}
	else if (_method == "HEAD" || _method == "PUT" || _method == "CONNECT" ||
			_method == "OPTIONS" || _method == "TRACE" || _method == "PATCH") {
		_ret = 501; //method not implemented
	}
	else {
		_ret = 400;
	}
}

void    Request::parsePath(std::stringstream &ss) {
	std::string requestPath;

	ss >> requestPath;
	if (requestPath.length() > 2048) {
		_ret = 414;
	}
	// space in uri is %20, so need decode it first
	while(true) {
		size_t pos = requestPath.find("%20");
		if (pos == std::string::npos) {
			break;
		}
		requestPath.replace(pos, 3, " ");
	}

	_path = requestPath;
	if (_path[0] != '/') {
		_ret = 400;
	}
}

void    Request::parseVersion(std::stringstream &ss) {
	ss >> _version;

	if (_version == "HTTP/2.0" || _version == "HTTP/2" || _version == "HTTP/3.0") {
		_ret = 505; //http version not supported
	}
	else if (_version != "HTTP/1.1") {
		_ret = 400;
	}
}

void    Request::parseHeaders(std::stringstream &ss) {
	std::string line;

	while (std::getline(ss, line)) {
		std::stringstream   header(line);
		if (line.empty() || line == "\r") {
			break;
		}
		std::string key, value;
		std::getline(header, key, ':');
		std::getline(header, value);
		value = value.substr(1,value.size() - 2);
		if (key.empty() || value.empty()) {
			_ret = 400;
		}
		_headers.insert(std::pair<std::string, std::string>(key, value));
	}
	if (_headers["Content-Type"].find("multipart/form-data") != std::string::npos) {
		_multipartBoundary = _headers["Content-Type"].substr(_headers["Content-Type"].find("boundary=----WebKitFormBoundary") + 31);
	}
	if (_headers.find(HOST) == _headers.end()) {
		_ret = 400;
	}
	else {
		_host = _headers[HOST];
	}
}

void    Request::parseBody(std::stringstream &ss, const std::string &requestStr) {
	int	currentPos = ss.tellg();
	if (currentPos != -1 && static_cast<unsigned long>(currentPos) < requestStr.size()) {
		std::string	restInput = requestStr.substr(currentPos, requestStr.size() - currentPos);
		_body.assign(restInput);
	}
	if (_multipartBoundary != "") {
		_fileName = _body.substr(_body.find("filename") + 10);
		_fileName = _fileName.substr(0, _fileName.find("\""));
		_fileContentType = _body.substr(_body.find("Content-Type:") + 14);
		_fileContentType = _fileContentType.substr(0, _fileContentType.find("\r"));
		for (int i = 0; i < 4; i++) {
			int line = _body.find_first_of("\n") + 1;
			_body = _body.substr(line, _body.size() - line);
		}
		for (int i = 0; i < 2; i++) {
			int line = _body.find_last_of("\n");
			_body = _body.substr(0, _body.size() - (_body.size() - line));
		}
		_body = _body.substr(0, _body.size() - 1);
	}

}

// Prints request info for debug option
void Request::printRequestDebug(const std::string &color) {
	std::cout << color << "[INFO] General " << std::endl;
	std::cout << "Request Method: " << _method << " -- HTTP Version: " << _version << std::endl;
	std::cout << "Path: " << _path << std::endl;
	std::cout << std::setfill('.') << std::setw(45) << "." << std::endl;
	std::cout << ITALIC "Request Headers:" RESET << std::endl;
	std::cout << color << "Host: " << _host << std::endl;
	std::map<std::string, std::string> headers = _headers;
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}
	std::cout << std::setfill('.') << std::setw(45) << "." << std::endl;
	std::cout << "Body: " << std::endl << GRAY << _body << RESET << std::endl;
	std::cout << color << "Body size: " << _body.size() << std::endl;
	std::cout <<  "------- End of request info --------\n" << RESET << std::endl;
}

// overload function
std::ostream &operator<<(std::ostream &os, const Request &request) {
	std::cout << YELLOW << "[INFO] Port: "
			  << request.getHost().substr(request.getHost().find_last_of(':') + 1,
										  request.getHost().size()) << " | "
			  << "Request Method: " << request.getMethod() << std::endl;
	os << "Path: " << request.getPath();
	return os;
}
