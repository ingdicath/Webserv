//
// Created by Hz Lin on 04/08/2022.
//

#ifndef WEBSERV_REQUESTTESTER_HPP
#define WEBSERV_REQUESTTESTER_HPP

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

class RequestTester {
public:
    RequestTester() {};
    ~RequestTester() {};

    int setupSocket(int port);

    // basic GET request, without body
    void    basicGetRequest(int &sock);
    // basic POST request, with content length and body (send as whole)
    void    basicPOSTRequest(int &sock);

    // basic invalid request tests
    void    invalidMethod(int &sock);
    void    uriTooLong(int &sock);
    void    invalidVersion(int &sock);
    void    headersTooLarge(int &sock); //bugs, check later

    // chunked, with content length
    void    chunkedRequest(int &sock);

	// chunked encoding, no content length but with hex num in each chunk
	void    chunkedEncoding(int &sock);
};

#endif //WEBSERV_REQUESTTESTER_HPP
