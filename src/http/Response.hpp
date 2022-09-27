//
// Created by Hz Lin on 16/08/2022.
//

#pragma once


#include <iostream>
#include <string>
#include <map>
#include <time.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm> //for linux
#include "../utils/settings.hpp"
#include "Request.hpp"
#include "HttpData.hpp"
#include "ResponseHeaders.hpp"

class Response {
private:
    std::string _path;
    std::string _method;
    HttpData    _httpData;
    bool        _closeConnection;
    int         _statusCode;
    std::string _type;
    long        _length;
    Location    _serverLocation;
    bool        _autoindex;
    std::string _body;

    Response();

    std::vector<std::string> setPathVector(std::string pathStr);
    int findRequestLocation();
    int responseValidation(Request &request);

    int isFile(const std::string &path);
    std::string autoIndexGenerator(std::string path, std::string dir);
    void    setErrorBody();

    void    processRedirection(Request &request);
    void    processGetMethod();
    void    processPostMethod(Request &request);
    void    processDeleteMethod();

    std::string getResponseStr(int code);

public:
    Response(HttpData &httpData, Request &request);
    virtual ~Response();
    Response(const Response &obj);
    Response &operator=(const Response &obj);

    //getters
    const std::string   &getPath() const;
    const std::string   &getMethod() const;
    const bool          &ifCloseConnection() const;
    const HttpData      &getHttpData() const;
    const int           &getStatusCode() const;
    const std::string   &getType() const;
    const std::string   &getBody() const;

    std::string getResponse(Request &request);

};

// for testing delete later
std::ostream	&operator<<(std::ostream &os, const Response &response);
