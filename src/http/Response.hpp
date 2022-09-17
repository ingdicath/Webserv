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
#include "../utils/settings.hpp"
#include "Request.hpp"
#include "HttpData.hpp"
#include "ResponseHeaders.hpp"

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


    Response(HttpData &httpData, Request &request);
    virtual ~Response();
    Response(const Response &obj);
    Response &operator=(const Response &obj);

    //getters
    const std::string   &getPath() const;
    const std::string   &getMethod() const;
    const bool          &ifCloseConnection() const;
    const HttpData      &getHttpData() const;
    const std::string   &getProtocol() const;
    const int           &getStatusCode() const;

    // generate the response string
    void        setErrorBody();
    void        processGetMethod(Request &request);
    void        processPostMethod(Request &request);
    std::string processDeleteMethod(Request &request, std::string location);
//    int         readContent(void);
//    int         writeContent(std::string content);
//    int         fileExists(std::string path);
    std::string getResponse(Request &request);

private:
    std::string _path;
    std::string _method;
    HttpData    _httpData;
    bool        _closeConnection;

    //status line
    std::string     _protocol;
    int             _statusCode;
    //headers zero or more, followed by CRLF
//    unsigned long   _length;
    std::string     _type;
    std::string     _location;
    std::string     _server;
    std::string     _date;
    std::string     _lastModified;
    //an empty line preceding the CRLF
    //body, optional
    std::string _body;

    Response();
    std::string getStatusMsg(int statusCode); //use it becaus cannot use the map
    int findRequestLocation();
    int isFile(const std::string &path);
};

// for testing delete later
std::ostream	&operator<<(std::ostream &os, const Response &reponse);
