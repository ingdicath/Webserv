//
// Created by Hz Lin on 16/08/2022.
//

#pragma once


#include <iostream>
#include <string>
#include <map>
#include <time.h>

class Response {
public:
    enum e_statusCode { //need more or less?
        //success
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        NO_CONTENT = 204,
        //redirection
        MOVED_PERMANENTLY = 301,
        FOUND = 302,
        SEE_OTHER = 303,
        TEMPORARY_REDIRECT = 307,
        //client errors
        BAD_REQUEST = 400,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        REQUEST_TIMEOUT = 408,
        GONE = 410,
        LENGTH_REQUIRED = 411,
        PAYLOAD_TOO_LARGE = 413,
        URI_TOO_LONG = 414,
        IM_A_TEAPOT = 418,
        TOO_MANY_REQUESTS = 429,
        REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
        //server errors
        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501,
        HTTP_VERSION_NOT_SUPPORTED = 505
    };

    static const std::map<int, const std::string>   statusMsg; // can't assign it with =, don't know why
    static const std::map<std::string, const std::string> contentType;

    Response();
    virtual ~Response();
    Response(const Response &obj);
    Response &operator=(const Response &obj);

    //add status, type, length, body in sequence
//    Response    &addStatus(const e_statusCode status);
//    Response    &addType(const std::string type);
//    Response    &addLength(const long contentLength);
//    Response    &addBody(const std::string body);

    std::string writeStatusLine(int statusCode);
    std::string writeHeaders();
    std::string writeBody();

private:
    //status line
    std::string _protocol;
    e_statusCode    _status;

    //headers zero or more, followed by CRLF
    long _length;
    std::string _type;
    bool _connection;
    std::string _location;
    std::string _server; //our own name?
    // std::string  _date;
    // std::string  _lastModified;
    //an empty line preceding the CRLF
    //body, optional
    std::string _body;

    std::string getStatusMsg(int statusCode); //use it becaus cannot use the map
};
