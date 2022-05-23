//
// Created by Diana catherine Salamanca leguizamon on 5/19/22.
//

#pragma once

#include <string>
#include <sstream>
#include <vector>

#define WHITESPACES " \v\t\n"

namespace utils {

	void removeWhiteSpaces(std::string str);

	std::vector<std::string> splitString(const std::string &str, char splitter);

	void trim(std::string &str, std::string chars);

}
