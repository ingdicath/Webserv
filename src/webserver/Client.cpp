/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/07 13:40:13 by aheister      #+#    #+#                 */
/*   Updated: 2022/07/20 16:19:52 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>

Client::Client(void) {
	return;
}

Client::Client(Client const & src) {
	*this = src;
}

Client& Client::operator=(Client const & rhs) {
	if (this != & rhs) {
		_clientSocket = rhs._clientSocket;
		_clientAddr = rhs._clientAddr;
		_clientTimeStamp = rhs._clientTimeStamp;
	}
	return *this;
}

Client::~Client(void) {
	return;
}

/*
** GET functions
*/
int	Client::getClientSocket(void) const {
	return _clientSocket;
}
struct sockaddr_in  Client::getClientAddress(void) const {
	return _clientAddr;
}

long long Client::getClientTimeStamp(void) const {
	return _clientTimeStamp;
}

/*
** SET functions
*/
void Client::setClientSocket(const int newSocket) {
	_clientSocket = newSocket;
}

void Client::setClientAddress(struct sockaddr_in newAddr) {
	_clientAddr = newAddr;
}

void Client::setClientTimeStamp(void) {
	struct	timeval tv;

	gettimeofday(&tv, NULL);
	_clientTimeStamp = tv.tv_sec;
}

//lin testing
static int chunkedEnd(const std::string &str){
    std::string endStr = "0\r\n\r\n";
    size_t i = str.size();
    size_t j = endStr.size();

    while (j > 0) {
        i--;
        j--;
        if (i < 0 || str[i] != endStr[j]) {
            return (1);
        }
    }
    return (0);
}

//return 0 is finished, return EXIT_FAILURE (1) when closing the conncetion
int     Client::recvRequest(int clientId) {
    char    buffer[MAXLINE + 1] = {0};
    int     ret;

    ret = recv(clientId, buffer, MAXLINE - 1, 0);
    if (ret == 0) {
        std::cout << "Connection was closed by client." << std::endl;
        return EXIT_FAILURE;
    }
    else if (ret == -1) {
        std::cout << "recv error, closing connection" << std::endl;
        return EXIT_FAILURE;
    }

    _request += std::string(buffer);

    size_t i = _request.find("\r\n\r\n"); // find the end of the headers
    if (i != std::string::npos) { //there is a body
        if (_request.find("Content-Length: ") == std::string::npos) { //no info about content length
            if (_request.find("Transfer-Encoding: chunked") != std::string::npos) { //it is chunked encoding
                std::cout << "CHUNKED" << std::endl;
                if (chunkedEnd(_request) == 0) { //chunked finished
                    return EXIT_SUCCESS;
                } else {
                    return -1; //chunked not finished
                }
            }
            else { //no content length and not chunked encoding
                return EXIT_SUCCESS;
            }
        }
        else { //there is content length
            size_t	len = std::atoi(_request.substr(_request.find("Content-Length: ") + 16, 10).c_str());
            if (_request.size() >= len + i + 4) {
                return EXIT_SUCCESS;
            } else {
                return -1; //content length does not match the body size
            }
        }
    }
    return -1;
}

void    Client::processChunk() {
    std::string headers = _request.substr(0, _request.find("\r\n\r\n"));
    std::cout << "HEADERS\n" << headers << std::cout;
    std::string	chunks = _request.substr(_request.find("\r\n\r\n") + 4, _request.size() - 1);
    std::string subChunk = chunks.substr(0,100);
    std::string body = "";
    int chunkSize = strtol(subChunk.c_str(), NULL, 16);
    size_t i = 0;

    while(chunkSize) {
        i = chunks.find("\r\n", i) + 2;
        body += chunks.substr(i, chunkSize);
        i += chunkSize + 2;
        subChunk = chunks.substr(i, 100);
        chunkSize = strtol(subChunk.c_str(), NULL, 16);
    }
    _request = headers + "\r\n\r\n" + body + "\r\n\r\n";
}

void    Client::processRequest() {
//    std::string recvd = "";

    if (_request.find("Transfer-Encoding: chunked") != std::string::npos &&
        _request.find("Transfer-Encoding: chunked") < _request.find("\r\n\r\n")) {
        std::cout << "test" << std::endl;
        processChunk();
    }

    //output for testing
    if (_request.size() < 1000) {
        std::cout << "\n_request:\n" << _request << std::endl;
    } else {
        std::cout << "\n_request:\n" << _request.substr(0, 1000) << std::endl;
    }

//    if (_request != "") {
//        Request request;
//    }
    _request = "";
}
