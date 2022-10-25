/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ResponseHeaders.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: hlin <hlin@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 10:39:07 by hlin          #+#    #+#                 */
/*   Updated: 2022/10/25 10:39:07 by hlin          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHeaders.hpp"

ResponseHeaders::ResponseHeaders() {
    _allow = "";
    _connection = "keep-alive";
    _contentLanguage = "en";
    _contentLength = "";
    _contentLocation = "";
    _contentType = "";
    _date = "";
    _lastModified = "";
    _location = "";
    _retryAfter = "";
    _server = "Webserv/1.0";
    _statusMsg[100] = "Continue";
    _statusMsg[200] = "OK";
    _statusMsg[201] = "Created";
    _statusMsg[204] = "No Content";
    _statusMsg[301] = "Moved Permanently";
    _statusMsg[302] = "Found";
    _statusMsg[303] = "See Other";
    _statusMsg[307] = "Temporary Redirect";
    _statusMsg[400] = "Bad Request";
    _statusMsg[403] = "Forbidden";
    _statusMsg[404] = "Not Found";
    _statusMsg[405] = "Method Not Allowed";
    _statusMsg[413] = "Payload Too Large";
    _statusMsg[414] = "URI Too Long";
	_statusMsg[415] = "Unsupported Media Type";
    _statusMsg[500] = "Internal Server Error";
    _statusMsg[501] = "Not Implemented";
    _statusMsg[502] = "Bad Gateway";
    _statusMsg[505] = "HTTP Version Not Supported";
}

ResponseHeaders::ResponseHeaders(const ResponseHeaders &obj) {
    *this = obj;
}

ResponseHeaders::~ResponseHeaders() {
}

ResponseHeaders &ResponseHeaders::operator=(const ResponseHeaders &obj) {
    if (this != &obj) {
        _allow = obj._allow;
        _connection = obj._connection;
        _contentLanguage = obj._contentLanguage;
        _contentLength = obj._contentLength;
        _contentLocation = obj._contentLocation;
        _contentType = obj._contentType;
        _date = obj._date;
        _lastModified = obj._lastModified;
        _location = obj._location;
        _retryAfter = obj._retryAfter;
        _server = obj._server;
        _statusMsg = obj._statusMsg;
    }
    return *this;
}

std::string  ResponseHeaders::numToStr(size_t n) {
    std::stringstream tmp;
    tmp << n;
    return tmp.str();
}

void    ResponseHeaders::setContentType(std::string type, std::string path) {
    if (type != "")
    {
        _contentType = type;
        return ;
    }
    type = path.substr(path.rfind(".") + 1, path.size() - path.rfind("."));
    if (type == "html" || type == "htm")
        _contentType = "text/html";
    else if (type == "css")
        _contentType = "text/css";
    else if (type == "js")
        _contentType = "text/javascript";
    else if (type == "jpeg" || type == "jpg")
        _contentType = "image/jpeg";
    else if (type == "png")
        _contentType = "image/png";
    else if (type == "bmp")
        _contentType = "image/bmp";
    else
        _contentType = "text/plain";
}

void    ResponseHeaders::setDate() {
    char			buffer[128];
    struct timeval	tv;
    struct tm		*tm;

    gettimeofday(&tv, NULL);
    tm = gmtime(&tv.tv_sec);
    strftime(buffer, 128, "%a, %d %b %Y %H:%M:%S GMT", tm);
    _date = std::string(buffer);
}

void    ResponseHeaders::setLastModified(const std::string &path) {
    char			buffer[128];
    struct stat		stats;
    struct tm		*tm;

    if (stat(path.c_str(), &stats) == 0) {
        tm = gmtime(&stats.st_mtime);
        strftime(buffer, 128, "%a, %d %b %Y %H:%M:%S GMT", tm);
        _lastModified = std::string(buffer);
    }
}

std::string		ResponseHeaders::getStatusMsg(int code) {
    if (_statusMsg.find(code) != _statusMsg.end())
        return _statusMsg[code];
    return "Unknown Status Code";
}

std::string		ResponseHeaders::writeHeader() {
    std::string	headers = "";
    if (_connection != "")
        headers += "Connection: " + _connection + "\r\n";
    if (_allow != "")
        headers += "Allow: " + _allow + "\r\n";
    if (_contentLanguage != "")
        headers += "Content-Language: " + _contentLanguage + "\r\n";
    if (_contentLength != "")
        headers += "Content-Length: " + _contentLength + "\r\n";
    if (_contentLocation != "")
        headers += "Content-Location: " + _contentLocation + "\r\n";
    if (_contentType != "")
        headers += "Content-Type: " + _contentType + "\r\n";
    if (_date != "")
        headers += "Date: " + _date + "\r\n";
    if (_lastModified != "")
        headers += "Last-Modified: " + _lastModified + "\r\n";
    if (_location != "")
        headers += "Location: " + _location + "\r\n";
    if (_retryAfter != "")
        headers += "Retry-After: " + _retryAfter + "\r\n";
    if (_server != "")
        headers += "Server: " + _server + "\r\n";
    return headers;
}

std::string ResponseHeaders::generateHeaderAllowed(int code, bool closeConnection, size_t size, std::string type,
                                                   std::string path, std::set<std::string> methods) {
    std::set<std::string>::iterator it = methods.begin();
    while (it != methods.end()) {
        _allow += *(it++);
        if (it != methods.end()) {
            _allow += ", ";
        }
    }
    if (closeConnection) {
        _connection = "close";
    }
    _contentLength = numToStr(size);
    _contentLocation = path;
    setContentType(type, path);
    setDate();

    std::string	headers;
    headers = "HTTP/1.1 " + numToStr(code) + " " + getStatusMsg(code) + "\r\n";
    headers += writeHeader();
    headers += "\r\n";
    return headers;
}

std::string ResponseHeaders::generateHeaderError(int code, bool closeConnection, size_t size,
                                                 std::string type, std::string path) {
    if (closeConnection) {
        _connection = "close";
    }
    _contentLength = numToStr(size);
    setContentType(type, path);
    setDate();
    if (code == 503 || code == 429) {
        _retryAfter = numToStr(10); //retry after 10 sec
    }

    std::string	headers;
    headers = "HTTP/1.1 " + numToStr(code) + " " + getStatusMsg(code) + "\r\n";
    headers += writeHeader();
    headers += "\r\n";
    return headers;
}

std::string ResponseHeaders::generateHeaderRedirection(int code, bool closeConnection, std::string location) {
    if (closeConnection) {
        _connection = "close";
    }
    _location = location;
    if (code == 301) {
        _retryAfter = numToStr(10); //retry after 10 sec
    }
    std::string	headers;
    headers = "HTTP/1.1 " + numToStr(code) + " " + getStatusMsg(code) + "\r\n";
    headers += writeHeader();
    headers += "\r\n";
    return headers;
}

std::string ResponseHeaders::generateHeader(int code, bool closeConnection, size_t size, std::string type,
                                            std::string path, std::string absolutePath) {
    if (closeConnection) {
        _connection = "close";
    }
    _contentLength = numToStr(size);
    if (path != "/autoindex") {
        _contentLocation = path;
    }
    setContentType(type, absolutePath);
    setLastModified(absolutePath);
    setDate();

    std::string	headers;
    headers = "HTTP/1.1 " + numToStr(code) + " " + getStatusMsg(code) + "\r\n";
    headers += writeHeader();
    headers += "\r\n";
    return headers;
}
