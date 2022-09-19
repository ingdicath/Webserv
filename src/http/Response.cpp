//
// Created by Hz Lin on 16/08/2022.
//

#include "Response.hpp"
#include "CGI.hpp"

Response::Response() {
}

Response::Response(HttpData &httpData, Request &request) :
        _httpData(httpData), _autoindex(false) {
    _path = request.getPath();
    _method = request.getMethod();
    _statusCode = request.getRet();
    _closeConnection = false;
    if (request.getHeaders().find("Connection") == request.getHeaders().end()) {
        if (request.getHeaders()["Connection"] == "close") {
            _closeConnection = true;
        }
    }
    _type = "";
    _body = "";
}

Response::~Response() {
}

Response::Response(const Response &obj) {
    *this = obj;
}

Response &Response::operator=(const Response &obj) {
    if (this != &obj) {
        _path = obj._path;
        _method = obj._method;
        _httpData = obj._httpData;
        _closeConnection = obj._closeConnection;
        _statusCode = obj._statusCode;
        _type = obj._type;
        _location = obj._location;
        _serverLocation = obj._serverLocation;
        _autoindex = obj._autoindex;
        _body = obj._body;
    }
    return *this;
}

const std::string   &Response::getPath() const {
    return _path;
}

const std::string   &Response::getMethod() const {
    return _method;
}
const HttpData  &Response::getHttpData() const {
    return _httpData;
}

const bool  &Response::ifCloseConnection() const {
    return _closeConnection;
}

const int   &Response::getStatusCode() const {
    return _statusCode;
}

const std::string &Response::getBody() const {
    return _body;
}

std::vector<std::string>    Response::setPathVector(std::string pathStr) {
    size_t i = 0;
    size_t slashPos;
    std::vector<std::string>    path;

    while (i < pathStr.length()) {
        slashPos = pathStr.find_first_of("/", i);
        if (slashPos == std::string::npos) {
            path.push_back(pathStr.substr(i, pathStr.length()));
            break;
        }
        else {
            if (i != slashPos - i) {
                path.push_back(pathStr.substr(i, slashPos - i));
            }
            path.push_back(pathStr.substr(slashPos, 1));
            i = slashPos + 1;
        }
    }
    return path;
}

int Response::findRequestLocation() {
    std::vector<Location>    locationVector = _httpData.getLocations();
    std::vector<std::string>    requestPath = setPathVector(_path);
    size_t lenRequestPath = requestPath.size();
    if (requestPath[lenRequestPath - 1] != "/") {
        lenRequestPath--;
    }

    for (size_t i = 0; i < locationVector.size(); i++) {
        std::vector<std::string>    locationPath = setPathVector(locationVector[i].getPathLocation());
        size_t lenLocationPath = locationPath.size();
        if (lenLocationPath == 1 && locationPath[0] == "/") {
            if (lenRequestPath == 1 && requestPath[0] == "/") {
                return static_cast<int>(i);
            } else {
                continue;
            }
        }
        if (lenLocationPath > lenRequestPath) {
            continue;
        }
        else {
            for (size_t j = 0; j < lenLocationPath; j++) {
                if (locationPath[j] != requestPath[j]) {
                    break;
                }
                if (j == lenLocationPath - 1) {
                    return static_cast<int>(i);
                }
            }
        }
    }
    return -1;
}

void    Response::setErrorBody() {
    _type = "text/html";
    std::map<int, std::string>	errorPage = _httpData.getErrorPages();
    if (errorPage.find(_statusCode) != errorPage.end()) {
        std::ofstream       file;
        std::stringstream   buffer;
        std::string         errorPagePath = "www" + errorPage[_statusCode];
        std::cout << "Error Page Path: " << errorPagePath << std::endl;
        file.open(errorPagePath.c_str(), std::ifstream::in);
        if (file.is_open() == false) {
            _body = "<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n";
        }
        else {
            buffer << file.rdbuf();
            file.close();
            _body = buffer.str();
        }
    }
    else {
        _body = "<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n";
    }
}

std::string Response::getResponse(Request &request) {
    std::string responseStr = "";
    ResponseHeaders  headers;
    int locationIndex = findRequestLocation();
    std::cout << "location index: " << locationIndex << std::endl; //testing
    if (locationIndex != -1) {
        _serverLocation = _httpData.getLocations()[locationIndex];
        _autoindex = _serverLocation.isAutoindex();
    }
    if (_statusCode == 200) {
        if (locationIndex == -1) {
            _statusCode = 500; // internal server error
        }
        else {
            if (_serverLocation.getAcceptedMethods().find(_method) == _serverLocation.getAcceptedMethods().end()) {
                _statusCode = 405;
            }
            else if (_httpData.getMaxClientBody() < request.getBody().size()) {
                _statusCode = 413;
                _closeConnection = true;
            }
        }
    }
    if (_statusCode == 405 || _statusCode == 413) {
        setErrorBody();
        if (_statusCode == 405) {
            responseStr = headers.getHeaderNotAllowed(_body.size(), _serverLocation.getAcceptedMethods(), _serverLocation.getPathLocation(), _statusCode, _type);
        } else {
            responseStr = headers.getHeaderError(_body.size(), _serverLocation.getPathLocation(), _statusCode, _type);
        }
        responseStr.append(_body);
        return responseStr;
    }
    else if (_statusCode >= 400) {
        setErrorBody();
        responseStr = headers.getHeaderError(_body.size(), _path, _statusCode, _type); //not sure if it is 500
        responseStr.append(_body);
        return responseStr;
    }
    else {
        if (_method == "GET") {
            processGetMethod();
        } else if (_method == "POST") {
            processPostMethod();
        } else if (_method == "DELETE") {
            processDeleteMethod(request);
        }
        if (_statusCode >= 200) {
            responseStr = headers.getHeaderError(_body.size(), _path, _statusCode, _type);
        } else {
            responseStr = headers.getHeader(_body.size(), _path, _statusCode, _type, _serverLocation.getPathLocation());
        }
        responseStr.append(_body);
        return responseStr;
    }
    return responseStr;
}

int Response::isFile(const std::string &path) { //return 1 if is file, return 2 if is directory
    struct stat s;
    if (stat(path.c_str(), &s) == 0 ) {
        if (s.st_mode & S_IFDIR) {
            return 2;
        }
        else if (s.st_mode & S_IFREG) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

std::string Response::autoIndexGenerator(std::string path, std::string directory) {
    DIR *dir = opendir(path.c_str());
    struct dirent* dirEntry;
    std::vector<std::string> dirVector;
    std::vector<std::string> fileVector;

    std::string res;
    res = "<!DOCTYPE html>\n<html>\n<head>\n" \
            "<title>" + directory + "</title>\n</head>\n";

    if (dir == NULL) {
        std::cerr << RED << "Error: could not open directory: " << dir << RESET << std::endl;
        res += "<body>\n<h1>Error: could not open directory</h1>\n</body>\n</html>\n";
        return res;
    }

    while ((dirEntry = readdir(dir)) != NULL) {
        if (dirEntry->d_type == DT_DIR) {
            std::string line = "<a href=\"" + directory + dirEntry->d_name + "/\">" + dirEntry->d_name + "/" + "</a><br>";
            dirVector.push_back(line);
        } else {
            std::string line = "<a href=\"" + directory + dirEntry->d_name + "\">" + dirEntry->d_name + "</a><br>";
            fileVector.push_back(line);
        }
    }
    closedir(dir);
    res += "<body>\n<h1> Index of " + directory + "</h1>\n<h2>";
    std::sort(dirVector.begin(), dirVector.end());
    for (std::vector<std::string>::iterator it = dirVector.begin(); it != dirVector.end(); it++) {
        res += *it;
    }
    std::sort(fileVector.begin(), fileVector.end());
    for (std::vector<std::string>::iterator it = fileVector.begin(); it != fileVector.end(); it++) {
        res += *it;
    }
    res += "</h2>\n</body>\n</html>\n";
    return res;
}

void Response::readContent() {
    std::ofstream       file;
    std::stringstream   buffer;
    std::string         contentPath = _serverLocation.getRoot();

    contentPath = contentPath + _path.substr(_serverLocation.getPathLocation().size() - 1);
    std::cout << "ContentPath: " << contentPath << std::endl; //testing
    int i = isFile(contentPath);
    if (i == 2) { //it is a dirctory
        // https://serverfault.com/questions/940276/force-nginx-to-always-autoindex-and-ignore-index-html-files
        if (isFile(contentPath + _serverLocation.getIndex()) == 0 && _autoindex) {
            std::cout << RED << autoIndexGenerator(contentPath, _path) << RESET << std::endl;
            _body = buffer.str();
            _type = "text/html";
            return;
        } else {
            contentPath = contentPath + _serverLocation.getIndex();
            i = isFile(contentPath);
        }
    }
    if (i > 0) {
        _path = contentPath;

        // CGI TEST
        std::string fileName = _path.substr(_path.find_last_of("/"));
        std::string fileExtension = fileName.substr(fileName.find_last_of("."));
        if (fileExtension == _serverLocation.getCgi().first) { //cgi part
            CGI cgi;
            cgi.addPath(fileName, _serverLocation);
            _body = cgi.execute();
            // insert function to find type and remove first 2 lines of _body
            _type = "text/html";
            return;
        }
        // How do we handle other extensions like .php and .bla which cannot be executed?
        file.open(contentPath.c_str(), std::ifstream::in);
        if (file.is_open() == false) {
            _statusCode = 403;
            setErrorBody();
        } else {
            buffer << file.rdbuf();
            _body = buffer.str();
            file.close();
        }
    }
    else {
        _statusCode = 404;
        setErrorBody();
    }
}

void     Response::processGetMethod() {
//    ResponseHeaders headers;

    if (_statusCode == 200) {
        readContent();
    } else {
        setErrorBody();
    }
}

void    Response::processPostMethod() { //need to implement more
//    ResponseHeaders headers;
    _statusCode = 204;
    _body = "";
}

void     Response::processDeleteMethod(Request &request) {
    ResponseHeaders headers;
    (void)request;

    _body = "";
    if (isFile(_path)) {
        if (remove(_path.c_str()) == 0) {
            _statusCode = 204;
        }
        else {
            _statusCode = 403;
        }
    }
    else {
        _statusCode = 404;
    }
    if (_statusCode == 403 || _statusCode == 404) {
        setErrorBody();
    }
}

// overload function for testing
std::ostream	&operator<<(std::ostream &os, const Response &response) {
    os << BLUE << "--------- Response Object Info ----------" << std::endl;
    os << "Path: " << response.getPath() << std::endl;
    os << "Method: " << response.getMethod() << std::endl;
    if (response.ifCloseConnection() == true) {
        os << "Close Connection: yes" << std::endl;
    } else {
        os << "Close Connection: no" << std::endl;
    }
    os << "StatusCode: " << response.getStatusCode() << std::endl;
//    os << "Body: \n" << response.getBody() << std::endl;
    os <<  "------- Response Object Info Done --------" << RESET << std::endl;
    return os;
}

