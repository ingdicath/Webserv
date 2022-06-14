//
// Created by Diana catherine Salamanca leguizamon on 5/19/22.
//

#pragma once

#include <string>
#include <sstream>
#include <vector>

#define WHITESPACES " \n\t\r\f\v"

namespace utils {

	void removeWhiteSpaces(std::string str); // check usage

	std::vector<std::string> splitString(const std::string &str, char splitter);  // check usage

	bool isPositiveNumber(const std::string &str);

	size_t stringToPositiveNum(const std::string &input);

	std::string &rTrim(std::string &str);

	std::string &lTrim(std::string &str);

	std::string &trim(std::string &str);

	bool validateIpAddress(const std::string &ipAddress);

	std::string deleteLastOf(char toSearch, std::string str);

}
