//
// Created by Hz Lin on 16/08/2022.
//

#include "Response.hpp"

std::string Response::getStatusMsg(int statusCode) {
    std::string statusMsg;
    if (statusCode == 200) {
        statusMsg = "OK";
    } else if (statusCode == 201) {
        statusMsg = "Created";
    } else if (statusCode == 202) {
        statusMsg = "Accepted";
    } else if (statusCode == 204) {
        statusMsg = "No Content";
    } else if (statusCode == 301) {
        statusMsg = "Moved Permanently";
    } else if (statusCode == 302) {
        statusMsg = "Found";
    } else if (statusCode == 303) {
        statusMsg = "See Other";
    } else if (statusCode == 307) {
        statusMsg = "Temporary Redirect";
    } else if (statusCode == 400) {
        statusMsg = "Bad Request";
    } else if (statusCode == 404) {
        statusMsg = "Not Found";
    } else if (statusCode == 403) {
        statusMsg = "Forbidden";
    } else if (statusCode == 405) {
        statusMsg = "Method Not Allowed";
    } else if (statusCode == 408) {
        statusMsg = "Request Timeout";
    } else if (statusCode == 410) {
        statusMsg = "Gone";
    } else if (statusCode == 411) {
        statusMsg = "Length Required";
    } else if (statusCode == 413) {
        statusMsg = "Payload Too Large";
    } else if (statusCode == 414) {
        statusMsg = "URI Too Long";
    } else if (statusCode == 418) {
        statusMsg = "I'm a teapot";
    } else if (statusCode == 429) {
        statusMsg = "Too Many Requests";
    } else if (statusCode == 431) {
        statusMsg = "Request Header Fields Too Large";
    } else if (statusCode == 500) {
        statusMsg = "Internal Server Error";
    } else if (statusCode == 501) {
        statusMsg = "Not Implemented";
    } else if (statusCode == 505) {
        statusMsg = "HTTP Version Not Supported";
    }
    return statusMsg;
}

//const std::map<int, const std::string> Response::statusMsg =
//        {
//                { 200, "OK" },
//                { 201, "Created" },
//                { 202, "Accepted" },
//                { 204, "No Content" },
//                { 301, "Moved Permanently" },
//                { 302, "Found" },
//                { 303, "See Other" },
//                { 307, "Temporary Redirect" },
//                { 400, "Bad Request" },
//                { 403, "Forbidden" },
//                { 404, "Not Found" },
//                { 405, "Method Not Allowed" },
//                { 408, "Request Timeout" },
//                { 410, "Gone" },
//                { 411, "Length Required" },
//                { 413, "Payload Too Large" },
//                { 414, "URI Too Long" },
//                { 418, "I'm a teapot" },
//                { 429, "Too Many Requests" },
//                { 431, "Request Header Fields Too Large" },
//                { 500, "Internal Server Error" },
//                { 501, "Not Implemented" },
//                { 505, "HTTP Version Not Supported" }
//                // ...
//        };

Response::Response() {
    _protocol = "HTTP/1.1";
}

Response::~Response() {
}

Response::Response(const Response &obj) {
    *this = obj;
}

Response &Response::operator=(const Response &obj) {
    if (this != &obj) {
        _protocol = obj._protocol;
        _statusCode = obj._statusCode;
//    _length = obj._length;
//    _type = obj._type;
//    _location = obj._location;
//    _server = obj._server;
        _body = obj._body;
    }
    return *this;
}

void    Response::setPort(int port) {
    _port = port;
}

void    Response::setHost(std::string host) {
    _host = host;
}

void    Response::setAutoIndex(bool autoIndex) {
    _autoIndex = autoIndex;
}

void    Response::setPath(std::string path) {
    _path = path;
}

void    Response::setMethod(std::string method) {
    _method = method;
}

void    Response::setStatusCode(int code) {
    _statusCode = code;
}

void    Response::setErrorPages(std::map<int, std::string> errorPages) {
    _errorPages = errorPages;
}

bool    Response::getAutoindex() const {
    return _autoIndex;
}

const int   &Response::getPort() const {
    return _port;
}

const std::string   &Response::getHost() const {
    return _host;
}

const std::string   &Response::getPath() const {
    return _path;
}

const std::string   &Response::getMethod() const {
    return _method;
}

const int   &Response::getStatusCode() const {
    return _statusCode;
}

const std::map<int, std::string> &Response::getErrorPages() const {
    return _errorPages;
}


static std::string  codeToStr(int code) {
    std::string res;
    for (int i = 0; i < 3; i++) {
        char c = (code % 10) + '0';
        res.insert(0, 1, c);
        code = code / 10;
    }
    return res;
}

std::string Response::writeStatusLine(int statusCode) {
    std::string statusMsg = getStatusMsg(statusCode);

    std::string statusLine;
    statusLine.append(_protocol);
    statusLine.append(" ");
    statusLine.append(codeToStr(statusCode));
    statusLine.append(" ");
    statusLine.append(statusMsg);
    statusLine.append("\r\n");
    return statusLine;
}

std::string Response::writeHeaders() {
    std::string headers;

    headers.append("\r\n");
    return headers;
}

std::string Response::writeBody() {
    _body = "<HTML>Hello</HTML>";

    std::string body = _body;
    return body;
}

std::string Response::getResponse() {
    std::string responseStr = writeStatusLine(200);
    responseStr.append(writeHeaders());
    responseStr.append(writeBody());
    return responseStr;
}

// overload function for testing
std::ostream	&operator<<(std::ostream &os, const Response &response) {
    os << BLUE << "--------- Response Object Info ----------" << std::endl;
    os << "Port: " << response.getPort() << std::endl;
    os << "Host: " << response.getHost() << std::endl;
    if (response.getAutoindex() == true) {
        os << "AutoIndex: true" << std::endl;
    } else {
        os << "AutoIndex: false" << std::endl;
    }
    os << "Path: " << response.getPath() << std::endl;
    os << "Method: " << response.getMethod() << std::endl;
    os << "StatusCode: " << response.getStatusCode() << std::endl;
    os << "ErrorPages:" << std::endl;
    std::map<int, std::string>	errorPages = response.getErrorPages();
    for(std::map<int, std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
        os << it->first << ": " << it->second << std::endl;
    }
    os <<  "------- Response Object Info Done --------" << RESET << std::endl;
    return os;
}

