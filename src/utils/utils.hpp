//
// Created by Diana catherine Salamanca leguizamon on 5/19/22.
//

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>

#define WHITESPACES " \n\t\r\f\v"

namespace utils {

	void 						removeWhiteSpaces(std::string str); // check usage
	std::vector<std::string>	splitString(const std::string &str, char splitter);
	bool 						isPositiveNumber(const std::string &str);
	size_t 						stringToPositiveNum(const std::string &input);
	std::string 				&rTrim(std::string &str);
	std::string 				&lTrim(std::string &str);
	std::string 				&trim(std::string &str);
	std::string 				deleteLastOf(char toSearch, std::string str);
	std::string 				stringToUpper(std::string oString);
	std::string 				stringToLower(std::string oString);

	int 						stringToInt(const std::string &input);
	long 						stringToLong(const std::string &input);
	std::map<int, std::string>	makeMap(int num, std::string string);
	std::vector<std::string>	splitByWhiteSpaces(const std::string &string, const std::string &whiteSp);
}
