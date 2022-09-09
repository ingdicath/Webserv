//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#pragma once

#include <vector>
#include <string>
#include <set>
#include <map>

class Location {
public:
	Location();
	explicit Location(std::string path);
	Location(Location const &obj);
	Location &operator=(Location const &obj);
	virtual ~Location();

	//Setters
	void setAutoindex(bool autoindex);
	void setMethods(const std::set<std::string> &acceptedMethods);
	void setRoot(const std::string &root);
	void setCGI(const std::pair<std::string, std::string> &cgi);
	void setUpload(const std::string &upload);
	void setIndex(const std::string &index);

	void addRedirection(const std::pair<int, std::string> &redirection);

private:
	std::string _pathLocation;
	std::string _upload;
	std::string _root;
	std::string _index;
	bool _autoindex;
	std::set<std::string> _acceptedMethods;
	std::map<int, std::string> _redirection;
	std::pair<std::string, std::string> _cgi;

	void	_setDefaultRedirectionPages();
	void	_setDefaultAllowedMethods();
	void	_setDefaultRoot();
};
