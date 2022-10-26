/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpData.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: hlin <hlin@student.codam.nl>          	      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 10:34:51 by hlin      	   #+#    #+#                 */
/*   Updated: 2022/10/25 10:34:51 by hlin          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "../webserver/Location.hpp"

class HttpData {
private:
    int             _port;
    std::string     _host;
    unsigned long   _maxClientBody;
    std::string     _serverName;
    std::map<int, std::string>  _errorPage;
    std::vector<Location>       _locations;

public:
    HttpData();
    HttpData(const HttpData &obj);
    virtual ~HttpData();
    HttpData &operator=(const HttpData &obj);

    //setters
    void    setPort(int port);
    void    setHost(std::string host);
    void    setMaxClientBody(unsigned long maxClientBody);
    void    setServerName(std::string serverName);
    void    setErrorPages(std::map<int, std::string> errorPages);
    void    setLocations(std::vector<Location> locations);

    //getters
    const int           &getPort() const;
    const std::string   &getHost() const;
    const unsigned long &getMaxClientBody() const;
    const std::string   &getServerName() const;
    const std::map<int, std::string> &getErrorPages() const;
    const std::vector<Location>      &getLocations() const;
};
