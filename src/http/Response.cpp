/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: hlin <hlin@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 10:38:10 by hlin          #+#    #+#                 */
/*   Updated: 2022/10/29 15:27:04 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "CGI.hpp"

Response::Response() {
}

Response::Response(HttpData &httpData, Request &request) :
		_httpData(httpData), _autoindex(false) {
	_path = request.getPath();
	_method = request.getMethod();
	_statusCode = request.getRet();
	if (_statusCode == 200 && httpData.getServerName() == "NF") {
		_statusCode = 404; //server not found
	}
	_type = "";
	_length = -1;
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
		_statusCode = obj._statusCode;
		_type = obj._type;
		_length = obj._length;
		_serverLocation = obj._serverLocation;
		_autoindex = obj._autoindex;
		_body = obj._body;
	}
	return *this;
}

int Response::isFile(const std::string &path) { //return 1 if is file, return 2 if is directory
    struct stat s;
    if (stat(path.c_str(), &s) == 0 ) {
        if (s.st_mode & S_IFREG) {
            return 1;
        }
        else if (s.st_mode & S_IFDIR) {
            return 2;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

//find if the path does not end with a / is a directory or not
//if it is, return ture for rediection in the future
bool    Response::isRedirection(const std::string &path) {
    bool needRedirect = false;
    char cwd[MAXLINE];
    char *res = getcwd(cwd, sizeof(cwd));
    std::string absolutePath = res;
    absolutePath = absolutePath.append(DEFAULT_ROOT);
    absolutePath = absolutePath.append(path);
    if (isFile(absolutePath) == 2) {
        needRedirect = true;
    }
    return needRedirect;
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

int Response::responseValidation(Request &request) {
    //if the _path (at this moment we get it from the request) end without a '/'
    //find the absolute path of it, then check if it is a directory
    bool needRedirect = false;
    if (_path[_path.length() - 1] != '/') {
        needRedirect = isRedirection(_path);
    }

    int locationIndex = findRequestLocation();
	if (locationIndex == -1) {
		return 404;
	} else {
		_serverLocation = _httpData.getLocations()[locationIndex];
		_autoindex = _serverLocation.isAutoindex();
		if (_serverLocation.getAcceptedMethods().find(_method) == _serverLocation.getAcceptedMethods().end()) {
			return 405;
		}
		if (_httpData.getMaxClientBody() < request.getBody().size() && _httpData.getMaxClientBody() != 0) {
			return 413;
		}
		if (_method == "POST") { //have to have content type and content length
			std::map <std::string, std::string> headers = request.getHeaders();
			if (headers.find("Content-Type") == headers.end()) {
				return 400;
			} else {
				_type = headers["Content-Type"];
			}
			if (headers.find("Content-Length") == headers.end()) {
				if (headers.find("Transfer-Encoding") == headers.end()) {
					return 411;
				} else {
					if (headers["Transfer-Encoding"] == "chunked") {
						_length = request.getBody().size();
					} else { //not chunked encoding
						return 411;
					}
				}
			} else {
				std::string tmp = headers["Content-Length"];
				_length = strtol(tmp.c_str(), NULL, 10);
			}
		}
        //e.g, /localhost"8085/images will be redirect to http://localhost:8085/images/
        if (needRedirect && _serverLocation.getRedirection().first == -1) {
            std::string redirectionUrl = "http://" + request.getHost() + _path + "/";
            std::cout << "Redirection: " << redirectionUrl << std::endl;
            _serverLocation.setRedirection(std::make_pair(301, redirectionUrl));
        }
	}
	return 200;
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

void    Response::setErrorBody() {
	_type = "text/html";
	std::map<int, std::string>	errorPage = _httpData.getErrorPages();
	if (errorPage.find(_statusCode) != errorPage.end()) {
		std::ofstream       file;
		std::stringstream   buffer;
		std::string         errorPagePath = "www" + errorPage[_statusCode];
		file.open(errorPagePath.c_str(), std::ifstream::in);
		if (file.is_open() == false) {
			_body = "<!DOCTYPE html>\n<html><title>404</title><body>There was an error finding your error page</body></html>\n";
		}
		else {
			buffer << file.rdbuf();
			file.close();
			_body = buffer.str();
		}
	}
	else {
		_body = "<!DOCTYPE html>\n<html><title>404</title><body>There was an error finding your error page</body></html>\n";
	}
}

void Response::processRedirection(Request &request) {
	std::string redirectionLocation = _serverLocation.getRedirection().second;
	size_t pos = redirectionLocation.find("$uri");
	if (pos != std::string::npos) {
		redirectionLocation.erase(pos, 4);
		redirectionLocation.append(request.getPath());
	}
	_path = redirectionLocation;
	_statusCode = _serverLocation.getRedirection().first;
	_body = "";
}

int Response::processCgiGet(std::string fileExtension, std::string contentPath, std::string queryString) {
	std::map<std::string, std::string> _cgiMethods = _serverLocation.getCgiMap();
	for (std::map<std::string, std::string>::iterator it_cgi = _cgiMethods.begin(); it_cgi != _cgiMethods.end(); it_cgi++) {
		if (fileExtension == it_cgi->first) {
			CGI cgi(GET, _httpData, contentPath);
			if (cgi.execute_GET(queryString) != 200) {
				_statusCode = cgi.getErrorCode();
				return 0;
			}
			std::istringstream input;
			_body = cgi.getBody();
			input.str(cgi.getBody());
			std::string line;
			std::getline(input, line, '\n');
			std::string toErase = "Content-type: ";
			line.erase(0, toErase.length());
			_type = line;
			_body = _body.substr(_body.find_first_of("<"));
			return 0;
		}
	}
	return 1;
}

void Response::processGetMethod() {
	std::ofstream       file;
	std::stringstream   buffer;
	std::string         contentPath = _serverLocation.getRoot();
	std::string         queryString;

	contentPath = contentPath + _path.substr(_serverLocation.getPathLocation().size() - 1);
	std::size_t pos = contentPath.find('?');
	if (pos != std::string::npos) {
		queryString = contentPath.substr(pos+1);
		contentPath = contentPath.substr(0, contentPath.find("?"));
	}
	int i = isFile(contentPath);
	if (i == 2) { //it is a directory
		// https://serverfault.com/questions/940276/force-nginx-to-always-autoindex-and-ignore-index-html-files
		if (isFile(contentPath + _serverLocation.getIndex()) == 0 && _autoindex) {
			_body = autoIndexGenerator(contentPath, _path);
			_type = "text/html";
			_path = contentPath + "autoindex";
			return;
		} else {
			contentPath = contentPath + _serverLocation.getIndex();
			i = isFile(contentPath);
		}
	}
	if (i > 0) {
		_path = contentPath;
		std::string fileExtension;
		size_t	dot = contentPath.find_last_of(".");
		if (dot != std::string::npos) {
			fileExtension = contentPath.substr(contentPath.find_last_of("."));
		}
		std::map<std::string, std::string> _cgiMethods = _serverLocation.getCgiMap();
		if (!fileExtension.empty() && !_cgiMethods.empty()) {
			if (processCgiGet(fileExtension, contentPath, queryString) == 0) {
				return;
			}
		}
		file.open(contentPath.c_str(), std::ifstream::in);
		if (file.is_open() == false) {
			_statusCode = 403;
		} else {
			buffer << file.rdbuf();
			_body = buffer.str();
			file.close();
		}
	}
	else {
		_statusCode = 404;
	}
}

int    Response::processCgiPost(Request &request, std::string fileExtension, std::string filePath) {
	std::map<std::string, std::string> _cgiMethods = _serverLocation.getCgiMap();
	for (std::map<std::string, std::string>::iterator it_cgi = _cgiMethods.begin(); it_cgi != _cgiMethods.end(); it_cgi++) {
		if (fileExtension == it_cgi->first) {
			CGI cgi(POST, _httpData, filePath);
			if (cgi.execute_POST(_type, request.getBody()) != 200) {
				_statusCode = cgi.getErrorCode();
				return 0;
			}
			_body = cgi.getBody();
			std::istringstream input;
			input.str(cgi.getBody());
			std::string line;
			std::getline(input, line, '\n');
			std::string toErase = "Content-type: ";
			line.erase(0, toErase.length());
			_type = line;
			_body = _body.substr(_body.find_first_of("<"));
			_path = filePath;
			return 0;
		}
	}
	return 1;
}

void    Response::processPostMethod(Request &request) {
	std::string filePath = _serverLocation.getRoot() + _path.substr(_serverLocation.getPathLocation().size() - 1);
	std::string fileExtension;
	size_t	dot = filePath.find_last_of(".");
	if (dot != std::string::npos) {
		fileExtension = filePath.substr(filePath.find_last_of("."));
	}
	std::map<std::string, std::string> _cgiMethods = _serverLocation.getCgiMap();
	if (!fileExtension.empty() && !_cgiMethods.empty()) {
		if (processCgiPost(request, fileExtension, filePath) == 0) {
			return;
		}
	}
	std::map <std::string, std::string> requestHeaders = request.getHeaders();
	std::vector<Location> locations = _httpData.getLocations();
	if (!request.getFileName().empty()) {
		for (std::vector<Location>::iterator itLocation = locations.begin();
			 itLocation < locations.end(); itLocation++) {
			if (itLocation->getUpload() != "") {
				filePath = _serverLocation.getRoot() + itLocation->getUpload() + "/" + request.getFileName();
			}
		}
	}
	if (isFile(filePath) == 1) { //file already exists
		_statusCode = 403;
		setErrorBody();
	} else {
		std::string dirPath = filePath.substr(0, filePath.find_last_of('/'));
		if (isFile(dirPath) != 2) { //could not find the directory to create this file
			std::cout << RED << dirPath << RESET << std::endl; // testing
			_statusCode = 403;
			return;
		}

		std::ofstream file;
		file.open(filePath.c_str(), std::ios::out | std::ios::binary);
		if (file.is_open() == false) {
			_statusCode = 403;
			return;
		} else {
			file << request.getBody();
			file.close();
			_statusCode = 201;
			_path = filePath;
			_body = "<html><body><h1>File created at URL: " + filePath + "</h1></body></head></html>";
			_type = "text/html";
		}
	}
}

void     Response::processDeleteMethod() {
	std::string filePath = _serverLocation.getRoot() + _path.substr(_serverLocation.getPathLocation().size() - 1);
	if (isFile(filePath) ==  1) {
		if (std::remove(filePath.c_str()) == 0) {
			_statusCode = 200;
			_body = "<html><body><h1>File deleted at URL: " + _path + "</h1></body></head></html>";
			_type = "text/html";
			_path = filePath;
		}
		else {
			_statusCode = 403;
		}
	}
	else {
		_statusCode = 404;
	}
}

std::string Response::getResponseStr(int code) {
	std::string res;
	ResponseHeaders  headers;

	if (code == 405) {
		res = headers.generateHeaderAllowed(code, _body.size(), _type, _path, _serverLocation.getAcceptedMethods());
	} else if (code >= 400) {
		res = headers.generateHeaderError(code, _body.size(), _type, _path);
	} else if (code / 100 == 3) {
		res = headers.generateHeaderRedirection(code, _path);
	} else {
		std::string contentPath = _path.substr(_serverLocation.getRoot().size());
		res = headers.generateHeader(code, _body.size(), _type, contentPath, _path);
	}
	if (DEBUG){
		std::cout << YELLOW << "[INFO] Response Headers: \n" << res  << std::endl;
		std::cout <<  "------- End of Response info --------\n" << RESET << std::endl;
	} else {
		std::cout << BLUE "Response Status Code: " << code << " " << headers.getStatusMsg(code) << std::endl << std::endl;
	}
	if (_body != "") {
		res += _body;
	}
	return res;
}

std::string Response::getResponse(Request &request) {
	if (_statusCode == 200) {
		_statusCode = responseValidation(request);
	}
	if (_statusCode != 200) {
		setErrorBody();
		return getResponseStr(_statusCode);
	}
	else {
		if (_serverLocation.getRedirection().first != -1) {
			processRedirection(request);
		} else if (_method == "GET") {
			processGetMethod();
		} else if (_method == "POST") {
			processPostMethod(request);
		} else if (_method == "DELETE") {
			processDeleteMethod();
		}
		if (_statusCode >= 400) {
			setErrorBody();
		}
		return getResponseStr(_statusCode);
	}
}
