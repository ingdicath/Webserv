//
// Created by Hz Lin on 16/08/2022.
//

#pragma once


#include <iostream>
#include <string>
#include <map>
#include <time.h>
#include "../utils/settings.hpp"
#include "Request.hpp"

class Response {
public:
//    enum e_statusCode { //need more or less?
//        //success
//        OK = 200,
//        CREATED = 201,
//        ACCEPTED = 202,
//        NO_CONTENT = 204,
//        //redirection
//        MOVED_PERMANENTLY = 301,
//        FOUND = 302,
//        SEE_OTHER = 303,
//        TEMPORARY_REDIRECT = 307,
//        //client errors
//        BAD_REQUEST = 400,
//        FORBIDDEN = 403,
//        NOT_FOUND = 404,
//        METHOD_NOT_ALLOWED = 405,
//        REQUEST_TIMEOUT = 408,
//        GONE = 410,
//        LENGTH_REQUIRED = 411,
//        PAYLOAD_TOO_LARGE = 413,
//        URI_TOO_LONG = 414,
//        IM_A_TEAPOT = 418,
//        TOO_MANY_REQUESTS = 429,
//        REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
//        //server errors
//        INTERNAL_SERVER_ERROR = 500,
//        NOT_IMPLEMENTED = 501,
//        HTTP_VERSION_NOT_SUPPORTED = 505
//    };

//    static const std::map<int, const std::string>   statusMsg; // can't assign it with =, don't know why
//    static const std::map<std::string, const std::string> contentType;

    Response();
    virtual ~Response();
    Response(const Response &obj);
    Response &operator=(const Response &obj);

    std::string writeStatusLine(int statusCode);
    std::string writeHeaders();
    std::string writeBody();

    std::string getResponse();

    //setters
    void    setPort(int port);
    void    setHost(std::string host);
    void    setAutoIndex(bool autoIndex);
    void    setPath(std::string path);
    void    setMethod(std::string method);
    void    setStatusCode(int code);
    void    setErrorPages(std::map<int, std::string> errorPages);

    //getters
    bool                getAutoindex() const;
    const int           &getPort() const;
    const std::string   &getHost() const;
    const std::string   &getPath() const;
    const std::string   &getMethod() const;
    const int           &getStatusCode() const;
    const std::map<int, std::string> &getErrorPages() const;


private:
    int         _port;
    std::string _host;
    bool        _autoIndex;
    std::string _path;
    std::string _method;

    //status line
    std::string     _protocol;
//    e_statusCode    _status;
    int             _statusCode;
    std::map<int, std::string>  _errorPages;

    //headers zero or more, followed by CRLF
//    long _length;
//    std::string _type;
//    bool _connection;
//    std::string _location;
//    std::string _server;
    // std::string  _date;
    // std::string  _lastModified;
    //an empty line preceding the CRLF
    //body, optional
    std::string _body;

    std::string getStatusMsg(int statusCode); //use it becaus cannot use the map
};

// for testing delete later
std::ostream	&operator<<(std::ostream &os, const Response &reponse);

