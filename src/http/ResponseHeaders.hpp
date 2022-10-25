/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ResponseHeaders.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: hlin <hlin@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 10:39:36 by hlin          #+#    #+#                 */
/*   Updated: 2022/10/25 10:39:36 by hlin          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "../utils/settings.hpp"
#include <set>
#include <map>
#include <sys/stat.h>
#include <sstream>
#include <sys/time.h>
#include "Request.hpp"

class ResponseHeaders {
private:
    std::string					_allow; //used for 405, Method not allowed
    std::string                 _connection; //close or keep-alive
    std::string					_contentLanguage;
    std::string					_contentLength;
    std::string					_contentLocation; //indicate the direct URL to use to access the resource
    std::string					_contentType;
    std::string					_date;
    std::string					_lastModified;
    std::string					_location; //the URL of a redirect
    std::string					_retryAfter; //413 and 301
    std::string					_server; //info about the software used by the origin server to handle the request
    std::map<int, std::string>	_statusMsg;

    std::string numToStr(size_t n);

    void    setContentType(std::string type, std::string path);
    void    setDate();
    void    setLastModified(const std::string& path);

    std::string writeHeader();

public:
    ResponseHeaders();
    ResponseHeaders(const ResponseHeaders &obj);
    virtual ~ResponseHeaders();
    ResponseHeaders &operator=(const ResponseHeaders &obj);

	std::string getStatusMsg(int code);
	std::string generateHeaderAllowed(int code, bool closeConnection, size_t size, std::string type, std::string path, std::set<std::string> methods);
    std::string generateHeaderError(int code, bool closeConnection, size_t size, std::string type, std::string path);
    std::string generateHeaderRedirection(int code, bool closeConnection, std::string location);
    std::string generateHeader(int code, bool closeConnection, size_t size, std::string type, std::string path, std::string absolutePath);
};
