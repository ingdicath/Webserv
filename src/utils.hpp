//
// Created by Diana catherine Salamanca leguizamon on 5/19/22.
//

#pragma once

#include <string>
#include <sstream>
#include <vector>

#define WHITESPACES " \v\t\n"

namespace utils {

	static void removeWhiteSpaces(std::string str);

	static std::vector<std::string> splitString(const std::string &str, char splitter);

};

