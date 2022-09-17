//
// Created by Hz Lin on 16/09/2022.
//

#pragma once

#include <iostream>
#include "../utils/settings.hpp"
#include <set>
#include <map>
#include <sys/stat.h>
#include <sstream>
#include <sys/time.h>

class ResponseHeaders {
public:
    ResponseHeaders();
    ResponseHeaders(const ResponseHeaders &obj);
    virtual ~ResponseHeaders();
    ResponseHeaders &operator=(const ResponseHeaders &obj);

    void    setContentType(std::string type, std::string path);
    void    setDate();
    void    setLastModified(const std::string& path);
    void    setAllow(std::set<std::string> methods);
    void    setHeaders(size_t size, const std::string &path, int code, std::string type, const std::string &contentLocation);

    std::string getStatusMsg(int code);
    std::string writeHeader();
    std::string getHeader(size_t size, const std::string &path, int code, std::string type, const std::string &contentLocation);
    std::string getHeaderNotAllowed(size_t size, std::set<std::string> methods, const std::string &path, int code, std::string type);
    std::string getHeaderError(size_t size, const std::string &path, int code, std::string type);

private:
    std::string					_allow;
    std::string                 _connection;
    std::string					_contentLanguage;
    std::string					_contentLength;
    std::string					_contentLocation;
    std::string					_contentType;
    std::string					_date;
    std::string					_lastModified;
    std::string					_location;
    std::string					_retryAfter;
    std::string					_server;
    std::string					_transferEncoding;
    std::map<int, std::string>	_errors;

    std::string numToStr(size_t n);
};
