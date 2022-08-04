//
// Created by Hz Lin on 04/08/2022.
//

#include "RequestTester.hpp"

#define PORT 80

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
        "host: localhost\r\n"\
        "\r\n";
    send(sock, rqs.c_str(), strlen(rqs.c_str()), 0);
    std::cout << "basic GET Request sent" << std::endl;

    int byteRead;
    char buff[1024] = { 0 };
    byteRead = read(sock, buff, 1024);
    std::cout << buff << std::endl;
    std::cout << "basic GET Request test done" << std::endl;
    return;
}

void    RequestTester::basicPOSTRequest(int &sock) {
    std::string rqs =
        "POST /basicPOSTRequest.txt HTTP/1.1\r\n"\
        "content-length: 24\r\n"\
        "host: localhost\r\n"\
        "\r\n"\
        "the body of this request";
    send(sock, rqs.c_str(), strlen(rqs.c_str()), 0);
    std::cout << "basic POST Request sent" << std::endl;

    int byteRead;
    char buff[1024] = {0};
    byteRead = read(sock, buff, 1024);
    std::cout << buff << std::endl;
    std::cout << "basic POST Request test done" << std::endl;
    return;
}

int main() {
    RequestTester   tester;

    int sock = tester.setupSocket(PORT);
    if (sock == -1) {
        return 1;
    }
    else {
//        tester.basicGetRequest(sock);
        tester.basicPOSTRequest(sock);
    }
    return (0);
}
