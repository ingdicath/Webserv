//
// Created by Diana catherine Salamanca leguizamon on 5/19/22.
//

#pragma once

#include <string>
#include <sstream>
#include <vector>

#define WHITESPACES " \n\t\r\f\v"

namespace utils {

	void removeWhiteSpaces(std::string str);

	std::vector<std::string> splitString(const std::string &str, char splitter);

	bool isPositiveNumber(const std::string &str);

	size_t stringToNum(const std::string &input);

	std::string &rtrim(std::string &str);

	std::string &ltrim(std::string &str);

	std::string &trim(std::string &str);

}
