//
// Created by Diana catherine Salamanca leguizamon on 5/24/22.
//

#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>

class Server {
public:

private:

	/** server **/
//	int _id;
	std::set<std::string> _listen;
	std::vector<std::string> _server_name;
//	std::vector<LocationBlock> _locations;
	std::vector<std::string> _cgi; // in case we decide accept other formats than .py, could be php?

//	/** location */
//	std::map<std::size_t, std::string> _error_page;
//	std::size_t _client_max_body_size;
//	std::vector<std::string> _index;
//	std::string _root;
//	bool _accepted_methods[3]; // list of constants
//	bool _autoindex;
//	std::map<std::string, std::string> _cgi; // podria ser tambien un vector??
//	std::string _upload;
//	std::string _redirection; //podria ser un pair std::pair<int, std::string>
};
