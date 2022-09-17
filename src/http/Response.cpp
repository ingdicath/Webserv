//
// Created by Hz Lin on 16/08/2022.
//

#include "Response.hpp"

Response::Response() {
}

Response::Response(HttpData &httpData, Request &request) :
        _httpData(httpData), _closeConnection(false), _protocol("HTTP/1.1") {
    _path = request.getPath();
    _method = request.getMethod();
    _statusCode = request.getRet();
    _body = "";
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

const std::string   &Response::getPath() const {
    return _path;
}

const std::string   &Response::getMethod() const {
    return _method;
}
const HttpData  &Response::getHttpData() const {
    return _httpData;
}

const bool  &Response::ifCloseConnection() const {
    return _closeConnection;
}

const std::string   &Response::getProtocol() const {
    return _protocol;
}

const int   &Response::getStatusCode() const {
    return _statusCode;
}

int Response::findRequestLocation() { //may have to do it with a vector
    std::vector<Location>    locationVector = _httpData.getLocations();
    std::string requestPath = _path;
    std::cout << "requestPath: " << requestPath << std::endl;

    for(size_t i = 0; i < locationVector.size(); i++) {
        std::string locationPath = locationVector[i].getPathLocation();
        std::cout << "locationPath: " << locationPath << " [" << i << "]" << std::endl;
        if (locationPath == requestPath) {
            return static_cast<int>(i);
        }
        else if (locationPath[locationPath.size() - 1] == '/' && locationPath.size() != 1) {
            if (locationPath.substr(0, (locationPath.size() - 1)) == requestPath) {
                return static_cast<int>(i);
            }
        }
    }
    return -1;
}

void    Response::setErrorBody() {
    _type = "text/html";
    std::map<int, std::string>	errorPage = _httpData.getErrorPages();
    if (errorPage.find(_statusCode) != errorPage.end()) {
        std::ofstream       file;
        std::stringstream   buffer;
        std::string         errorPagePath = "www" + errorPage[_statusCode];
        std::cout << "Error Page Path: " << errorPagePath << std::endl;
        file.open(errorPagePath.c_str(), std::ifstream::in);
        if (file.is_open() == false) {
            _body = "<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n";
        }
        else {
            buffer << file.rdbuf();
            file.close();
            _body = buffer.str();
        }
    }
    else {
        _body = "<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n";
    }
}

std::string Response::getResponse(Request &request) {
    std::string responseStr = "";
    ResponseHeaders  headers;
    int locationIndex = findRequestLocation();
    std::cout << "location index: " << locationIndex << std::endl; //testing
    if (_statusCode == 200) {
        if (locationIndex == -1) {
            _statusCode = 500; // internal server error
        }
        else {
            Location    location = _httpData.getLocations()[locationIndex];
            if (location.getAcceptedMethods().find(_method) == location.getAcceptedMethods().end()) {
                _statusCode = 405;
            }
            else if (_httpData.getMaxClientBody() < request.getBody().size()) {
                _statusCode = 413;
            }
        }
    }
    if (_statusCode == 405 || _statusCode == 413) {
        Location    location = _httpData.getLocations()[locationIndex];
        setErrorBody();
        if (_statusCode == 405) {
            responseStr = headers.getHeaderNotAllowed(_body.size(), location.getAcceptedMethods(), location.getPathLocation(), _statusCode, _type);
        } else {
            responseStr = headers.getHeaderError(_body.size(), location.getPathLocation(), _statusCode, _type);
        }
        responseStr.append(_body);
        return responseStr;
    }
    else if (_statusCode != 200) {
        setErrorBody();
        responseStr = headers.getHeaderError(_body.size(), _path, _statusCode, _type); //not sure if it is 500
        responseStr.append(_body);
        return responseStr;
    }
    else {
        responseStr = "Http/1.1 200 OK\r\n<HTML>Hello</HTML>";
    }
    return responseStr;
}

int Response::isFile(const std::string &path) {
    struct stat s;
    if (stat(path.c_str(), &s) == 0 ) {
        if (s.st_mode & S_IFDIR) {
            return 0;
        }
        else if (s.st_mode & S_IFREG) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

std::string    Response::processDeleteMethod(Request &request, std::string location) {
    ResponseHeaders headers;
    (void)request;
    std::string     responseStr;

    _body = "";
    if (isFile(_path)) {
        if (remove(_path.c_str()) == 0) {
            _statusCode = 204;
        }
        else {
            _statusCode = 403;
        }
    }
    else {
        _statusCode = 404;
    }
    if (_statusCode == 403 || _statusCode == 404) {
        setErrorBody();
    }
    responseStr = headers.getHeader(_body.size(), _path, _statusCode, _type, location);
    responseStr.append(_body);
    return responseStr;
}

// overload function for testing
std::ostream	&operator<<(std::ostream &os, const Response &response) {
    os << BLUE << "--------- Response Object Info ----------" << std::endl;
    os << "Path: " << response.getPath() << std::endl;
    os << "Method: " << response.getMethod() << std::endl;
    if (response.ifCloseConnection() == true) {
        os << "Close Connection: yes" << std::endl;
    } else {
        os << "Close Connection: no" << std::endl;
    }
    os << "StatusCode: " << response.getStatusCode() << std::endl;
    os << "HttpData:" << response.getHttpData() << std::endl;
    os <<  "------- Response Object Info Done --------" << RESET << std::endl;
    return os;
}

