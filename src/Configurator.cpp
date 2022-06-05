//
// Created by Diana catherine Salamanca leguizamon on 5/31/22.
//

#include <vector>
#include <string>
#include "Configurator.hpp"
#include "Server.hpp"
#include "settings.hpp"

Configurator::Configurator() {}

Configurator::~Configurator() {}

// check if structures, Must be return in a new line??
Configurator::eDirectives Configurator::resolveDirective(const std::string &input) {
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

//std::pair<std::string, std::string> Configurator::splitDirective(const std::string &rawInput) {
//
//	std::string directive = rawInput.substr(0, rawInput.find(' '));
//	std::string value = rawInput.substr(rawInput.find(' '), rawInput.size());
//	return std::pair<std::string, std::string>(trim(directive), trim(value));
//}
//
//void Configurator::setServerDirectives(const std::string &rawInput, Server *server) {
//
//	std::pair<std::string, std::string> directive = splitDirective(rawInput);
//
//	switch (resolveDirective(directive.first)) {
//		case LISTEN:
//			server->setListenFromInput(directive.second);
//		case SERVER_NAME:
//			//TODO;
//			break;
//		case ERROR_PAGE:
//			//TODO;
//			break;
//		case BODY_SIZE:
//			//TODO;
//			break;
//		default:
//			throw std::runtime_error("Config error: unknown directive in server block" + rawInput);
//	}
//}

//bool Configurator::setLocationDirectives(const std::string &input) {
//
//	switch (resolveDirective(input)) {
//		case ROUTE_LOCATION:
//			return isValid(value);
//		case ROOT:
//			return ;
//		case ACCEPTED_METHODS:
//			return ;
//		case INDEX:
//			return ;
//		case AUTOINDEX:
//			return ;
//		case CGI:
//			return ;
//		case UPLOAD:
//			return ;
//		case REDIRECTION:
//			return ;
//		default:
//			throw std::runtime_error("Config error: unknown directive in location block" + directive, std::string &value);
//	}
//}

//const char *ws = " \t\n\r\f\v";
//const std::string ws = " \n\t\r\f\v";

//// trim from end of string (right)
//std::string &rtrim(std::string &s) {
//	std::string ws = " \n\t\r\f\v";
//	s.erase(s.find_last_not_of(ws) + 1);
//	return s;
//}
//
//// trim from beginning of string (left)
//std::string &ltrim(std::string &s) {
//	std::string ws = " \n\t\r\f\v";
//	s.erase(0, s.find_first_not_of(ws));
//	return s;
//}
//
//// trim from both ends of string (right then left)
//std::string &trim(std::string &s) {
//	return ltrim(rtrim(s));
//}


