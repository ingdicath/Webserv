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
    char        buffer[100];
    time_t      rawTime;
    struct tm   *tm;
    std::string headers = "";

    if (_closeConnection) {
        headers += "connection: Closed\r\n";
    }
//    if (_body != "") {
//        headers += "Content-Length: " + _body.length() + "\r\n";
//    }
//    headers += "Location: " + _path + "\r\n"; // may change after rediection
    headers += "Content-Type: " + _type + "\r\n";
    headers += "Server: " + _server + "\r\n";

    time(&rawTime);
    tm = gmtime(&rawTime);
    strftime(buffer, 100, "%a, %d %b %G %T GMT", tm);
    std::string timeStr = static_cast<std::string>(buffer);
    headers += "Date: " + timeStr + "\r\n";
    headers += "Last-Modified: " + timeStr + "\r\n"; //will change through modify
    headers.append("\r\n");
    return headers;
}

std::string Response::writeBody() {
    if (_body == "") {
        _body = "<HTML>Hello</HTML>";
    }
    return _body;
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
        _body = "<html><body><h1>" + codeToStr(_statusCode) + " " + getStatusMsg(_statusCode) + "</h1></body></head></html>";
    }
}

std::string Response::generateErrorResponse() {
    if (_statusCode == 413) {
        _closeConnection = true; //payload too large, need to close the connection
    }
    setErrorBody();
    std::string responseStr = writeStatusLine(_statusCode);
    responseStr.append(writeHeaders());
    responseStr.append(writeBody());
    return responseStr;
}

std::string Response::getResponse() {
    std::string responseStr = "";
    if (_statusCode != 200) { //there is an error
        responseStr.append(generateErrorResponse());
        return responseStr;
    }
    int locationIndex = findRequestLocation();
    std::cout << "location index: " << locationIndex << std::endl; //testing
    if (locationIndex == -1) {
        _statusCode = 500; // internal server error
    }
    // have to figure out what to do with redirection
//  else if (httpData.getLocations()[locationIndex].getRedirection()) {
//  }
    else {
       Location    location = _httpData.getLocations()[locationIndex];
       if (location.getAcceptedMethods().find(_method) == location.getAcceptedMethods().end()) {
           _statusCode = 405;
       }
    }
    if (_statusCode >= 400) { //there is an error
        responseStr.append(generateErrorResponse());
        return responseStr;
    }
    else {
        responseStr.append(writeStatusLine(200));
        responseStr.append(writeHeaders());
        responseStr.append(writeBody());
    }
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

