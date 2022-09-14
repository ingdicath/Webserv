//
// Created by Hz Lin on 14/09/2022.
//

#include "HttpData.hpp"

HttpData::HttpData() {
}

HttpData::HttpData(const HttpData &obj) {
    *this = obj;
}

HttpData::~HttpData() {
}

HttpData    &HttpData::operator=(const HttpData &obj) {
    if (this != &obj) {
        _port = obj._port;
        _host = obj._host;
        _maxClientBody = obj._maxClientBody;
        _path = obj._path;
        _serverName = obj._serverName;
        _errorPage = obj._errorPage;
        _locations = obj._locations;
    }
    return *this;
}

void    HttpData::setPort(int port) {
    _port = port;
}
void    HttpData::setHost(std::string host) {
    _host = host;
}

void    HttpData::setMaxClientBody(unsigned long maxClientBody) {
    _maxClientBody = maxClientBody;
}

void    HttpData::setPath(std::string path) {
    _path = path;
}

void    HttpData::setServerName(std::string serverName) {
    _serverName = serverName;
}

void    HttpData::setErrorPages(std::map<int, std::string> errorPages) {
    _errorPage = errorPages;
}

void    HttpData::setLocations(std::vector<Location> locations) {
    _locations = locations;
}

const int   &HttpData::getPort() const {
    return _port;
}

const std::string   &HttpData::getHost() const {
    return _host;
}

const unsigned long &HttpData::getMaxClientBody() const {
    return _maxClientBody;
}

const std::string   &HttpData::getPath() const {
    return _path;
}

const std::string   &HttpData::getServerName() const {
    return _serverName;
}

const std::map<int, std::string> &HttpData::getErrorPages() const {
    return _errorPage;
}

const std::vector<Location> &HttpData::getLocations() const {
    return _locations;
}
