//
// Created by Hz Lin on 18/07/2022.
//

#include "Request.hpp"
#include "../utils/settings.hpp"
#include "../utils/utils.hpp"

Request::Request() {
    _headersComplete = false;
    _chunked = false;
    _chunkedComplete = false;
    _chunkedHex = false;
    _chunkedSeparatedCRLF = false;
    _chunkedLength = 0;
}

Request::Request(long maxClientBody) {
    _maxClientBody = maxClientBody;
    _headersComplete = false;
    _chunked = false;
    _chunkedComplete = false;
    _chunkedHex = false;
    _chunkedSeparatedCRLF = false;
    _chunkedLength = 0;
}

Request::Request(const Request &obj) {
    *this = obj;
}

Request &Request::operator=(const Request &obj) {
    _rawRequest = obj._rawRequest;
    _headersComplete = obj._headersComplete;
    _contentLength = obj._contentLength;
    _chunked = obj._chunked;
    _chunkedComplete = obj._chunkedComplete;
    _chunkedHex = obj._chunkedHex;
    _chunkedSeparatedCRLF = obj._chunkedSeparatedCRLF;
    _chunkedLength = obj._chunkedLength;
    _method = obj._method;
    _path = obj._path;
    _httpVersion = obj._httpVersion;
    _headers = obj._headers;
    _body = obj._body;
    _host = obj._host;
    return *this;
}

Request::~Request() {
}

// Getters, for testing now, may use later for handling
Request::method  Request::getMethod() const {
    return _method;
}

std::vector<std::string>    Request::getPath() const {
    return _path;
}

std::string	Request::getVersion() const {
	return _httpVersion;
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

std::string Request::getRawRequest() const {
    return _rawRequest;
}

void    Request::parseMethod(std::stringstream &ss) {
    std::string method;

    ss >> method;
    if (method == "GET") {
        _method = GET;
    }
    else if (method == "POST") {
        _method = POST;
    }
    else if (method == "DELETE") {
        _method = DELETE;
    }
    else if (method == "HEAD" || method == "PUT" || method == "CONNECT" ||
            method == "OPTIONS" || method == "TRACE" || method == "PATCH") {
        throw MethodNotSupportedException();
    }
    else {
        throw MethodInvalidException();
    }
}

void Request::setPath(std::string line) {
    size_t i = 0;
    size_t slash;

    if (line[i] != '/') {
        return;
    }
    while (i < line.length()) {
        slash = line.find_first_of("/", i);
        if (slash == std::string::npos) {
            _path.push_back(line.substr(i, line.length()));
            break;
        }
        else {
            if (i != slash - i) {
                _path.push_back(line.substr(i, slash - i));
            }
            _path.push_back(line.substr(slash, 1));
            i = slash + 1;
        }
    }
}

void    Request::parsePath(std::stringstream &ss) {
    std::string requestPath;

    ss >> requestPath;
    if (requestPath.length() > 2048) {
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

    setPath(requestPath);
    if (_path[0] != "/") {
        throw UriInvalidException();
    }
}

void    Request::parseVersion(std::stringstream &ss) {
    ss >> _httpVersion;

    if (_httpVersion == "HTTP/2.0" || _httpVersion == "HTTP/2" || _httpVersion == "HTTP/3.0") {
        throw VersionNotSupportedException();
    }
    else if (_httpVersion != "HTTP/1.1") {
        throw VersionInvalidException();
    }
}

static std::string lowerCases(std::string str) {
    for (size_t i = 0; i < str.size(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
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
        key = lowerCases(key);
        std::getline(header, value);
        value = value.substr(1,value.size() - 2);
        if (key.empty() || value.empty()) {
            throw HeadersIncorrectException();
        }
        _headers.insert(std::pair<std::string, std::string>(key, value));
    }

    if (_headers.find(TRANSFER_ENCODING) != _headers.end() && _headers[TRANSFER_ENCODING] == "chunked") {
        _chunked = true;
    }
    else if (_headers.find(CONTENT_LENGTH) == _headers.end()) {
        _contentLength = 0;
    }
    else {
        _contentLength = utils::strToLong(_headers[CONTENT_LENGTH].c_str());
    }

    if (_headers.find(HOST) == _headers.end()) {
        _host = "NAV";
    }
    else {
        _host = _headers[HOST];
    }
}

static long getHexNum(char c) {
    long    res = -1;

    if (c >= '0' && c <= '9') {
        res = c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
        res = c - 'A' + 10;
    }
    else if (c >= 'a' && c <= 'f') {
        res = c - 'a' + 10;
    }
    return res;
}

void    Request::parseBody(std::string &input, long len) {
    if (!_chunked) {
        _body.append(input.c_str(), len);
        if (static_cast<long>(_body.size()) > _contentLength) {
            throw BodyLengthIncorrectException();
        }
    }
    else {
        long i = 0;

        if (input[i] == '0') {
            _chunkedComplete = true;
            return;
        }
        while (input[i] != '\r' && getHexNum(input[i]) != -1) {
            _chunkedLength = _chunkedLength * 16 + getHexNum(input[i]);
            i++;
        }
        i += 2; // skip the \r\n

        std::cout << "chunkedLength: " << _chunkedLength << std::endl;

        if (_chunkedLength != len - i - 2) {
            throw BodyLengthIncorrectException(); //need check, may be a different exception
        }
        else {
            _body.append(input.substr(i, _chunkedLength));
            _chunkedLength = 0;
        }
    }
//    if (_contentLength != 0 && _contentLength > _maxClientBody) {
//        throw MaxClientBodyException();
//    }
}

int    Request::parseRequest(char rawRequest[], int bytesRead) {
    if (_headersComplete) {
        std::string newInput;
        for (int i = 0; i  < bytesRead; i++) {
            newInput += rawRequest[i];
        }
        parseBody(newInput, bytesRead);
    }
    else {
        _rawRequest.append(rawRequest, bytesRead);
    }

    if (_rawRequest.find("\r\n\r\n") != std::string::npos && !_headersComplete) {
        std::stringstream   ss(_rawRequest);

		// parse the request line: method, URI and HTTP version
		// throw exceptions when invalid, may need to change with response code
        try {
            parseMethod(ss);
            parsePath(ss);
            parseVersion(ss);
            ss.ignore(2); // skip the \r\n
            parseHeaders(ss);
            int	currentPos = ss.tellg();
            std::string	restInput = _rawRequest.substr(currentPos, _rawRequest.size() - currentPos);
            parseBody(restInput, restInput.size());
            _headersComplete = true;
        }
        catch (std::exception &e) {
			std::cout << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}

bool    Request::isComplete() const {
    if (!_headersComplete) {
        return false;
    }
    if (_chunked) {
        return (_chunkedComplete);
    }
    return (static_cast<long>(_body.size()) >= _contentLength);
}

// overload function for testing
std::ostream	&operator<<(std::ostream &os, const Request &request) {
	os << CYAN << "--------- Raw Request ---------" << std::endl;
	os << request.getRawRequest() << RESET << std::endl;
	
	os << BLUE << "--------- Request Object Info ----------" << std::endl;
	Request::method	method = request.getMethod();
	if (method == 0) {
		os << "Method: GET" << std::endl;
	} else if (method == 1) {
		os << "Method: POST" << std::endl;
	} else if (method == 2) {
		os << "Method: DELETE" << std::endl;
	}
	
	std::vector<std::string>	path = request.getPath();
 	os << "Path: ";
    for (int i = 0; i < path.size(); i++) {
        os << path[i] << " ";
    }
	os << std::endl;

	os << "HTTP Version: " << request.getVersion() << std::endl;

	os << "HEADERS:" << std::endl;
	std::map<std::string, std::string>	headers = request.getHeaders();
	for(std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        os << it->first << ": " << it->second << std::endl;
    }

	os << "Host: " << request.getHost() << std::endl;

	os << "Body: " << std::endl << request.getBody() << RESET << std::endl;

	return os;
}
