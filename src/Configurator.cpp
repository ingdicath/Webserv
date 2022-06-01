//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include <vector>
#include <string>
#include "Configurator.hpp"

Configurator::Configurator() {}

Configurator::~Configurator() {}

// check if structures, Must be return in a new line??
Configurator::eDirectives Configurator::resolveDirective(const std::string &input) {
//
//	std::string dirArray[] = {"listen", "location", "server_name",
//							  "error_page", "client_max_body_size", "index",
//							  "root", "accepted_methods", "autoindex",
//							  "cgi", "upload", "redirection", NULL};
//	std::vector<std::string> directives(dirArray, dirArray + dirArray->size());

	if (input == "listen") return LISTEN;
	if (input == "server_name") return SERVER_NAME;
	if (input == "error_page") return ERROR_PAGE;
	if (input == "client_max_body_size") return BODY_SIZE;
	if (input == "location") return ROUTE_LOCATION; // Check this
	if (input == "root") return ROOT;
	if (input == "accepted_methods") return ACCEPTED_METHODS;
	if (input == "index") return INDEX;
	if (input == "autoindex") return AUTOINDEX;
	if (input == "cgi") return CGI;
	if (input == "upload") return UPLOAD;
	if (input == "redirection") return REDIRECTION;
	return INVALID;
}

void Configurator::setServerDirectives(const std::string &input) {
	switch (resolveDirective(input)) {
		case LISTEN:
			//TODO;
			break;
		case SERVER_NAME:
			//TODO;
			break;
		case ERROR_PAGE:
			//TODO;
			break;
		case BODY_SIZE:
			//TODO;
			break;
		default:
			throw std::runtime_error("Config error: unknown directive in server block" + input);
	}
}

void Configurator::setLocationDirectives(const std::string &input) {
	switch (resolveDirective(input)) {
		case ROUTE_LOCATION:
			//TODO;
			break;
		case ROOT:
			//TODO;
			break;
		case ACCEPTED_METHODS:
			//TODO;
			break;
		case INDEX:
			//TODO;
			break;
		case AUTOINDEX:
			//TODO;
			break;
		case CGI:
			//TODO;
			break;
		case UPLOAD:
			//TODO;
			break;
		case REDIRECTION:
			//TODO;
			break;
		default:
			throw std::runtime_error("Config error: unknown directive in location block" + input);
	}
}
