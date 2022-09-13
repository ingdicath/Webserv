//
// Created by Hz Lin on 18/07/2022.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <sstream>

#define CONTENT_LENGTH "Content-Length"
#define HOST "Host"

class Request {
private:
    unsigned long                       _clientMaxBodySize;
    std::string                         _method;
    std::string                         _path;
    std::string                         _version;
    std::map<std::string, std::string>  _headers;
	std::string                         _host;
    std::string                         _body;
    int                                 _ret;

	void    parseMethod(std::stringstream &ss);
	void    parsePath(std::stringstream &ss);
	void    parseVersion(std::stringstream &ss);
	void    parseHeaders(std::stringstream &ss);
    void    parseBody(std::stringstream &ss, const std::string &requestStr);

    Request();

public:
    Request(const std::string &requesStr, unsigned long clientMaxBodySize);
    virtual ~Request();

    Request(const Request &obj);
    Request &operator=(const Request &obj);

    int parseRawRequest(const std::string &rawRequest);

    //getters
    std::string  getMethod() const;
    std::string getPath() const;
    std::string	getVersion() const;
	std::map<std::string, std::string>	getHeaders() const;
    std::string getHost() const;
    std::string getBody() const;
    int         getRet() const;

    class   HeadersIncorrectException : public std::exception {
    public:
        const char *what() const throw() {
            return "Incorrect HTTP headers";
        }
    };

    class   MethodInvalidException : public std::exception {
    public:
        const char *what() const throw() {
            return "Invalid method";
        }
    };

    class   MethodNotSupportedException : public std::exception {
    public:
        const char *what() const throw() {
            return "This method is not supported";
        }
    };

    class   VersionInvalidException : public std::exception {
    public:
        const char *what() const throw() {
            return "Invalid HTTP version";
        }
    };

    class   VersionNotSupportedException : public std::exception {
    public:
        const char *what() const throw() {
            return "This HTTP version is not supported";
        }
    };

    class   UriInvalidException : public std::exception {
    public:
        const char *what() const throw() {
            return "URI format invalid";
        }
    };

    class   UriTooLongException : public std::exception {
    public:
        const char *what() const throw() {
            return "URI too long";
        }
    };

    class   MaxClientBodyException : public std::exception {
    public:
        const char *what() const throw() {
            return "Exceeded max client body";
        }
    };

    class   BodyLengthIncorrectException : public std::exception {
    public:
        const char *what() const throw() {
            return "Body length does not match content length";
        }
    };
};

// for testing delete later
std::ostream	&operator<<(std::ostream &os, const Request &request);
