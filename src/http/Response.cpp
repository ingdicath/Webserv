//
// Created by Hz Lin on 16/08/2022.
//

#include "Response.hpp"

std::string Response::getStatusMsg(int statusCode) {
    std::string statusMsg;
    if (statusCode == 200) {
        statusMsg = "OK";
    } else if (statusCode == 201) {
        statusMsg = "Created";
    } else if (statusCode == 202) {
        statusMsg = "Accepted";
    } else if (statusCode == 204) {
        statusMsg = "No Content";
    } else if (statusCode == 301) {
        statusMsg = "Moved Permanently";
    } else if (statusCode == 302) {
        statusMsg = "Found";
    } else if (statusCode == 303) {
        statusMsg = "See Other";
    } else if (statusCode == 307) {
        statusMsg = "Temporary Redirect";
    } else if (statusCode == 400) {
        statusMsg = "Bad Request";
    } else if (statusCode == 404) {
        statusMsg = "Not Found";
    } else if (statusCode == 403) {
        statusMsg = "Forbidden";
    } else if (statusCode == 405) {
        statusMsg = "Method Not Allowed";
    } else if (statusCode == 408) {
        statusMsg = "Request Timeout";
    } else if (statusCode == 410) {
        statusMsg = "Gone";
    } else if (statusCode == 411) {
        statusMsg = "Length Required";
    } else if (statusCode == 413) {
        statusMsg = "Payload Too Large";
    } else if (statusCode == 414) {
        statusMsg = "URI Too Long";
    } else if (statusCode == 418) {
        statusMsg = "I'm a teapot";
    } else if (statusCode == 429) {
        statusMsg = "Too Many Requests";
    } else if (statusCode == 431) {
        statusMsg = "Request Header Fields Too Large";
    } else if (statusCode == 500) {
        statusMsg = "Internal Server Error";
    } else if (statusCode == 501) {
        statusMsg = "Not Implemented";
    } else if (statusCode == 505) {
        statusMsg = "HTTP Version Not Supported";
    }
    return statusMsg;
}

//const std::map<int, const std::string> Response::statusMsg =
//        {
//                { 200, "OK" },
//                { 201, "Created" },
//                { 202, "Accepted" },
//                { 204, "No Content" },
//                { 301, "Moved Permanently" },
//                { 302, "Found" },
//                { 303, "See Other" },
//                { 307, "Temporary Redirect" },
//                { 400, "Bad Request" },
//                { 403, "Forbidden" },
//                { 404, "Not Found" },
//                { 405, "Method Not Allowed" },
//                { 408, "Request Timeout" },
//                { 410, "Gone" },
//                { 411, "Length Required" },
//                { 413, "Payload Too Large" },
//                { 414, "URI Too Long" },
//                { 418, "I'm a teapot" },
//                { 429, "Too Many Requests" },
//                { 431, "Request Header Fields Too Large" },
//                { 500, "Internal Server Error" },
//                { 501, "Not Implemented" },
//                { 505, "HTTP Version Not Supported" }
//                // ...
//        };

Response::Response() {
    _protocol = "HTTP/1.1";
    _server = "Connecting Kitten's Webserver";
}

Response::~Response() {
}

Response::Response(const Response &obj) {
    *this = obj;
}

Response &Response::operator=(const Response &obj) {
    _protocol = obj._protocol;
    _status = obj._status;
    _length = obj._length;
    _type = obj._type;
    _location = obj._location;
    _server = obj._server;
    _body = obj._body;
    return *this;
}

static std::string  codeToStr(int code) {
    std::string res;

    for (int i = 0; i < 3; i++) {
        res.insert(0, 1, ((code % 10) + '0'));
        code = code / 10;
    }
    return res;
}

std::string Response::writeStatusLine(int statusCode) {
    std::string statusMsg = getStatusMsg(statusCode);

    std::string statusLine;
    statusLine.append(_protocol);
    statusLine.append(" ");
    statusLine.append(codeToStr(statusCode));
    statusLine.append(" ");
    statusLine.append(statusMsg);
    statusLine.append("\r\n");
    return statusLine;
}

std::string Response::writeHeaders() {
    std::string headers;

    headers.append("\r\n");
    return headers;
}

std::string Response::writeBody() {
    _body = "<HTML>Hello</HTML>";

    std::string body = _body;
    return body;
}

std::string Response::getResponse() {
    std::string responseStr = writeStatusLine(200);
    responseStr.append(writeHeaders());
    responseStr.append(writeBody());
    return responseStr;
}

//int main() {
//    Response    response;
//    std::string output = response.writeStatusLine(200);
//    output.append(response.writeHeaders());
//    output.append(response.writeBody());
//    std::cout << output << std::endl;
//    return 0;
//}
