//
// Created by Hz Lin on 04/08/2022.
//

#include "RequestTester.hpp"

#define PORT 8085

int RequestTester::setupSocket(int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    sleep(2);
    return sock;
}

void    RequestTester::basicGetRequest(int &sock) {
    std::string rqs =
        "GET / HTTP/1.1\r\n"\
        "Host: localhost\r\n"\
        "\r\n";
    send(sock, rqs.c_str(), strlen(rqs.c_str()), 0);
    std::cout << "basic GET Request sent" << std::endl;

    int byteRead;
    char buff[1024] = { 0 };
    byteRead = read(sock, buff, 1024);
    if (byteRead != -1) {
        std::cout << buff << std::endl;
    }
    std::cout << "basic GET Request test done" << std::endl;
    return;
}

void    RequestTester::basicPOSTRequest(int &sock) {
    std::string rqs =
        "POST /basicPOSTRequest.txt HTTP/1.1\r\n"\
        "Content-Length: 24\r\n"\
        "Host: localhost\r\n"\
        "\r\n"\
        "the body of this request";
    send(sock, rqs.c_str(), strlen(rqs.c_str()), 0);
    std::cout << "basic POST Request sent" << std::endl;

    int byteRead;
    char buff[1024] = {0};
    byteRead = read(sock, buff, 1024);
    if (byteRead != -1) {
        std::cout << buff << std::endl;
    }
    std::cout << "basic POST Request test done" << std::endl;
    return;
}

void    RequestTester::longGetRequest(int &sock) {
    std::string rqs =
            "GET / HTTP/1.1\r\n"
            "Host: localhost:8085\r\n"
            "Connection: keep-alive\r\n"
            "sec-ch-ua: \"Chromium\";v=\"104\", \" Not A;Brand\";v=\"99\", \"Google Chrome\";v=\"104\"\r\n"
            "sec-ch-ua-mobile: ?0\r\n"
            "sec-ch-ua-platform: \"macOS\"\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.0.0 Safari/537.36\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
            "Sec-Fetch-Site: none\r\n"
            "Sec-Fetch-Mode: navigate\r\n"
            "Sec-Fetch-User: ?1\r\n"
            "Sec-Fetch-Dest: document\r\n"
            "Accept-Encoding: gzip, deflate, br\r\n"
            "Accept-Language: en-GB,en;q=0.9,nl;q=0.8,zh-CN;q=0.7,zh;q=0.6\r\n"
            "\r\n";
    send(sock, rqs.c_str(), strlen(rqs.c_str()), 0);
    std::cout << "long GET Request sent" << std::endl;

    int byteRead;
    char buff[1024] = { 0 };
    byteRead = read(sock, buff, 1024);
    if (byteRead != -1) {
        std::cout << buff << std::endl;
    }
    std::cout << "long GET Request test done" << std::endl;
    return;
}

void    RequestTester::longPOSTRequest(int &sock) {
    std::string rqs =
            "POST /basicPOSTRequest.txt HTTP/1.1\r\n"\
        "Content-Length: 445\r\n"\
        "Host: localhost\r\n"\
        "\r\n"\
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    send(sock, rqs.c_str(), strlen(rqs.c_str()), 0);
    std::cout << "long POST Request sent" << std::endl;

    int byteRead;
    char buff[1024] = {0};
    byteRead = read(sock, buff, 1024);
    if (byteRead != -1) {
        std::cout << buff << std::endl;
    }
    std::cout << "long POST Request test done" << std::endl;
    return;
}

void    RequestTester::invalidMethod(int &sock) {
    std::string rqs =
            "TEST / HTTP/1.1\r\n"\
		"Host: localhost\r\n"\
		"\r\n";
    send(sock, rqs.c_str(), strlen(rqs.c_str()), 0);
    std::cout << "Invalid Request method sent" << std::endl;

    int byteRead;
    char buff[1024] = {0};
    byteRead = read(sock, buff, 1024);
    if (byteRead != -1) {
        std::cout << buff << std::endl;
    }
    std::cout << "Invalid Request method test done" << std::endl;
    return;
}

void    RequestTester::uriTooLong(int &sock) {
    std::string rqs =
        "GET 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 HTTP/1.1\r\n"\
		"Host: localhost\r\n"\
		"\r\n";
    send(sock, rqs.c_str(), strlen(rqs.c_str()), 0);
    std::cout << "Invalid Request URI too long sent" << std::endl;

    int byteRead;
    char buff[1024] = {0};
    byteRead = read(sock, buff, 1024);
    if (byteRead != -1) {
        std::cout << buff << std::endl;
    }
    std::cout << "Invalid Request URI too long test done" << std::endl;
    return;
}

void    RequestTester::invalidVersion(int &sock) {
    std::string rqs =
            "GET / HTTP/2\r\n"\
		"Host: localhost\r\n"\
		"\r\n";
    send(sock, rqs.c_str(), strlen(rqs.c_str()), 0);
    std::cout << "Invalid Request HTTP version sent" << std::endl;

    int byteRead;
    char buff[1024] = {0};
    byteRead = read(sock, buff, 1024);
    if (byteRead != -1) {
        std::cout << buff << std::endl;
    }
    std::cout << "Invalid Request HTTP version test done" << std::endl;
    return;
}

void    RequestTester::chunkedRequest(int &sock) {
    std::string rqsFirst =
        "POST /chunksTest.txt HTTP/1.1\r\n"\
		"Host: localhost\r\n"\
		"Content-Length: 64\r\n"\
		"\r\n"\
		"the first chunk of this request\n";
    std::string rqsSecond = "the second chunk of this request";

    send(sock, rqsFirst.c_str(), strlen(rqsFirst.c_str()), 0);
    std::cout << "Request with chunked body: first chunk sent" << std::endl;
    sleep(1);
    send(sock, rqsSecond.c_str(), strlen(rqsSecond.c_str()), 0);
    std::cout << "Request with chunked body: second chunk sent" << std::endl;

    int byteRead;
    char buff[1024] = { 0 };
    byteRead = read(sock, buff, 1024);
    if (byteRead != -1) {
        std::cout << buff << std::endl;
    }
    std::cout << "Request with chunked body test done" << std::endl;
    return;
}

void    RequestTester::chunkedEncoding(int &sock) {
	std::string rqsFirst =
        "POST /chunksTest.txt HTTP/1.1\r\n"\
		"host: localhost\r\n"\
        "Transfer-Encoding: chunked\r\n"\
		"\r\n"\
		"4\r\nWiki\r\n";
    std::string rqsSecond = "6\r\npedia \r\n";
	std::string rqsThird = "E\r\nin \r\n\r\nchunks.\r\n";
	std::string rqsFinal = "0\r\n\r\n";

    send(sock, rqsFirst.c_str(), strlen(rqsFirst.c_str()), 0);
    std::cout << "Request with chunked encoding: first chunk sent" << std::endl;
    sleep(1);
    send(sock, rqsSecond.c_str(), strlen(rqsSecond.c_str()), 0);
    std::cout << "Request with chunked encoding: second chunk sent" << std::endl;
    sleep(1);
	send(sock, rqsThird.c_str(), strlen(rqsThird.c_str()), 0);
    std::cout << "Request with chunked encoding: third chunk sent" << std::endl;
    sleep(1);
	send(sock, rqsFinal.c_str(), strlen(rqsFinal.c_str()), 0);
    std::cout << "Request with chunked encoding: final chunk sent" << std::endl;

    int byteRead;
    char buff[1024] = { 0 };
    byteRead = read(sock, buff, 1024);
    if (byteRead != -1) {
        std::cout << buff << std::endl;
    }
    std::cout << "Request with chunked encoding test done" << std::endl;
    return;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "pass the indicator of the test as argument" << std::endl;
        std::cout << "1:  basic GET request, without body" << std::endl;
        std::cout << "2:  basic POST request, with content_length and body" << std::endl;
        std::cout << "3:  Long GET request" << std::endl;
        std::cout << "4:  Long GET request" << std::endl;
        std::cout << "5:  Request with invalid method" << std::endl;
        std::cout << "6:  Request with an URI too long" << std::endl;
        std::cout << "7:  Request with invalid HTTP version" << std::endl;
        std::cout << "8:  Request with chunked body" << std::endl;
		std::cout << "9:  Request with chunked encoding" << std::endl;
        return 1;
    }

    RequestTester   tester;
    int sock = tester.setupSocket(PORT);
    if (sock == -1) {
        return 1;
    }
    else {
        if (std::strcmp(argv[1], "1") == 0) {
            tester.basicGetRequest(sock);
        }
        else if (std::strcmp(argv[1], "2") == 0) {
            tester.basicPOSTRequest(sock);
        }
        else if (std::strcmp(argv[1], "3") == 0) {
            tester.longGetRequest(sock);
        }
        else if (std::strcmp(argv[1], "4") == 0) {
            tester.longPOSTRequest(sock);
        }
        else if (std::strcmp(argv[1], "5") == 0) {
            tester.invalidMethod(sock);
        }
        else if (std::strcmp(argv[1], "6") == 0) {
            tester.uriTooLong(sock);
        }
        else if (std::strcmp(argv[1], "7") == 0) {
            tester.invalidVersion(sock);
        }
        else if (std::strcmp(argv[1], "8") == 0) {
            tester.chunkedRequest(sock);
        }
		else if (std::strcmp(argv[1], "9") == 0) {
            tester.chunkedEncoding(sock);
        }
        else {
            std::cout << "Invalid test indicator" << std::endl;
            return 1;
        }
    }
    return (0);
}
