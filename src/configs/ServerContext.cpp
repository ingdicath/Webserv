//
// Created by Diani on 04/08/2022.
//

#include "ServerContext.hpp"
#include "../utils/utils.hpp"
#include "../utils/settings.hpp"

ServerContext::~ServerContext() {}

ServerContext::ServerContext() {}

bool ServerContext::_isValidPortRange(const std::string &port) {
	size_t portNumber = utils::stringToPositiveNum(port);
	if (portNumber < MIN_PORT_NUMBER || portNumber > MAX_PORT_NUMBER) {
		throw InvalidPortRangeException();
//		throw std::runtime_error("Config error: invalid port value: '" + port + "'");
	}
	return true;
}

bool ServerContext::_isValidIpv4Address(const std::string &ipAddress) {
	if (ipAddress == "localhost") {
		return true;
	}
	size_t numDots = std::count(ipAddress.begin(), ipAddress.end(), '.');
	std::vector<std::string> vec = utils::splitString(ipAddress, '.');
	if (numDots > 3 || vec.size() != 4) {
		return false;
	}
	size_t num;
	for (size_t i = 0; i < vec.size(); i++) {
		num = utils::stringToPositiveNum(vec.at(i));
		if (num > 255) {
			return false;
		}
	}
	return true;
}

bool ServerContext::_isValidServerName(const std::string &serverName) {
	if (serverName[0] == '/' || serverName[0] == '*') {
		throw std::runtime_error("Config error: path can't be a directory or use wildcard *. ");
	}
	if (serverName.find_last_of('/') == serverName.size() - 1 && serverName.size() != 1) {
		throw std::runtime_error("Config error: path can't be a directory.");
	}
	return true;
}

