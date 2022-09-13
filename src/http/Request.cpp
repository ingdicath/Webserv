//
// Created by Hz Lin on 18/07/2022.
//

#include "Request.hpp"
#include "../utils/settings.hpp"
#include "../utils/utils.hpp"

Request::Request() {
}

Request::Request(const std::string &requestStr, unsigned long clientMaxBodySize) :
    _clientMaxBodySize(clientMaxBodySize), _body(""), _ret(200) {
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

int Request::parseRawRequest(const std::string &rawRequest) {
    if (rawRequest.find("\r\n\r\n") != std::string::npos) {
        try {
            std::stringstream ss(rawRequest);
            parseMethod(ss);
            parsePath(ss);
            parseVersion(ss);
            ss.ignore(2); // skip the \r\n
            parseHeaders(ss);
            parseBody(ss, rawRequest);
        }
        catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
    else {
        _ret = 400;
    }
    return _ret;
}

// Getters, for testing now, may use later for handling
std::string  Request::getMethod() const {
    return _method;
}

std::string    Request::getPath() const {
    return _path;
}

std::string	Request::getVersion() const {
	return _version;
}

std::map<std::string, std::string>	Request::getHeaders() const {
	return _headers;
}

std::string Request::getHost() const {
    return _host;
}

std::string Request::getBody() const {
    return _body;
}

int Request::getRet() const {
    return _ret;
}


void    Request::parseMethod(std::stringstream &ss) {
    ss >> _method;
    if (_method == "GET" || _method == "POST" || _method == "DELETE") {
        return;
    }
    else if (_method == "HEAD" || _method == "PUT" || _method == "CONNECT" ||
            _method == "OPTIONS" || _method == "TRACE" || _method == "PATCH") {
        _ret = 501;
        throw MethodNotSupportedException();
    }
    else {
        _ret = 400;
        throw MethodInvalidException();
    }
}

void    Request::parsePath(std::stringstream &ss) {
    std::string requestPath;

    ss >> requestPath;
    if (requestPath.length() > 2048) {
        _ret = 414;
        throw UriTooLongException();
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
        throw UriInvalidException();
    }
}

void    Request::parseVersion(std::stringstream &ss) {
    ss >> _version;

    if (_version == "HTTP/2.0" || _version == "HTTP/2" || _version == "HTTP/3.0") {
        _ret = 505;
        throw VersionNotSupportedException();
    }
    else if (_version != "HTTP/1.1") {
        _ret = 400;
        throw VersionInvalidException();
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
//        key = lowerCases(key);
        std::getline(header, value);
        value = value.substr(1,value.size() - 2);
        if (key.empty() || value.empty()) {
            _ret = 400;
            throw HeadersIncorrectException();
        }
        _headers.insert(std::pair<std::string, std::string>(key, value));
    }

    if (_headers.find(HOST) == _headers.end()) {
        _host = "NAV";
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
    if (_body.size() != 0 && _clientMaxBodySize != 0 && _body.size() > _clientMaxBodySize) {
        _ret = 431;
        throw MaxClientBodyException();
    }
}

// overload function for testing
std::ostream	&operator<<(std::ostream &os, const Request &request) {
	os << BLUE << "--------- Request Object Info ----------" << std::endl;
    os << "Method: " << request.getMethod() << std::endl;
 	os << "Path: " << request.getPath() << std::endl;
	os << "HTTP Version: " << request.getVersion() << std::endl;

	os << "HEADERS:" << std::endl;
	std::map<std::string, std::string>	headers = request.getHeaders();
	for(std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        os << it->first << ": " << it->second << std::endl;
    }

	os << "Host: " << request.getHost() << std::endl;
	os << "Body: " << std::endl << request.getBody() << std::endl;
    os << "Ret:  " << request.getRet() << std::endl;
    os <<  "------- Request Object Info Done --------" << RESET << std::endl;
	return os;
}
