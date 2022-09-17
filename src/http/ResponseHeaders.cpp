//
// Created by Hz Lin on 16/09/2022.
//

#include "ResponseHeaders.hpp"

ResponseHeaders::ResponseHeaders() {
    _allow = "";
    _connection = "keep-alive";
    _contentLanguage = "";
    _contentLength = "";
    _contentLocation = "";
    _contentType = "";
    _date = "";
    _lastModified = "";
    _location = "";
    _retryAfter = "";
    _server = "";
    _transferEncoding = "";
    _errors[100] = "Continue";
    _errors[200] = "OK";
    _errors[201] = "Created";
    _errors[204] = "No Content";
    _errors[400] = "Bad Request";
    _errors[403] = "Forbidden";
    _errors[404] = "Not Found";
    _errors[405] = "Method Not Allowed";
    _errors[413] = "Payload Too Large";
    _errors[414] = "URI Too Long";
    _errors[500] = "Internal Server Error";
    _errors[501] = "Not Implemented";
    _errors[505] = "HTTP Version Not Supported";
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
        _transferEncoding = obj._transferEncoding;
        _errors = obj._errors;
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
    if (type == "html")
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

void    ResponseHeaders::setAllow(std::set<std::string> methods)
{
    std::set<std::string>::iterator it = methods.begin();
    while (it != methods.end())
    {
        _allow += *(it++);
        if (it != methods.end())
            _allow += ", ";
    }
}

void    ResponseHeaders::setHeaders(size_t size, const std::string &path, int code, std::string type, const std::string &contentLocation) {
    if (code == 413) {
        _connection = "close";
    }
    _contentLanguage = "en";
    _contentLength = numToStr(size);
    if (code != 404) {
        _contentLocation = contentLocation;
    }
    setContentType(type, path);
    setDate();
    setLastModified(path);
    if (code == 201 || code / 100 == 3) {
        _location = path;
    }
    if (code == 503 || code == 429 || code == 301) {
        _retryAfter = numToStr(3); //retry after 3 sec
    }
    _server = "Webserv/1.0";
    _transferEncoding = "identity";
}

std::string		ResponseHeaders::getStatusMsg(int code) {
    if (_errors.find(code) != _errors.end())
        return _errors[code];
    return "Unknown Code";
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
    if (_transferEncoding != "")
        headers += "Transfer-Encoding: " + _transferEncoding + "\r\n";
    return headers;
}

std::string ResponseHeaders::getHeader(size_t size, const std::string &path, int code, std::string type, const std::string &contentLocation) {
    std::string	headers;

    setHeaders(size, path, code, type, contentLocation);
    headers = "HTTP/1.1 " + numToStr(code) + " " + getStatusMsg(code) + "\r\n";
    headers += writeHeader();
    headers += "\r\n";
    return headers;
}

std::string		ResponseHeaders::getHeaderNotAllowed(size_t size, std::set<std::string> methods, const std::string &path, int code, std::string type)
{
    std::string	headers;

    setAllow(methods);
    setHeaders(size, path, code, type, path);
    headers = "HTTP/1.1 " + numToStr(code) + " " + getStatusMsg(code) + "\r\n";
    headers += writeHeader();
    headers += "\r\n";
    return headers;
}

std::string		ResponseHeaders::getHeaderError(size_t size, const std::string &path, int code, std::string type)
{
    std::string	headers;

    setHeaders(size, path, code, type, path);
    headers = "HTTP/1.1 " + numToStr(code) + " " + getStatusMsg(code) + "\r\n";
    headers += writeHeader();
    headers += "\r\n";
    return headers;
}