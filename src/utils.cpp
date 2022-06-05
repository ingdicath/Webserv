//
// Created by Diana catherine Salamanca leguizamon on 5/19/22.
//

#include <string>
#include <vector>
#include "utils.hpp"

void utils::removeWhiteSpaces(std::string str) {
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

// https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
std::vector<std::string> utils::splitString(const std::string &str, char splitter) {
	std::vector<std::string> res;
	std::stringstream line(str);
	std::string segment;

	while (std::getline(line, segment, splitter)) {
		res.push_back(segment);
	}
	return res;
}

bool utils::isPositiveNumber(const std::string &str) {
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it)) {
		it++;
	}
	return !str.empty() && it == str.end();
}

size_t utils::stringToNum(const std::string &input) {
	size_t res;
	std::stringstream ss(input);
	ss >> res;
	return res;
}

// trim from end of string (right)
std::string &utils::rtrim(std::string &str) {
	str.erase(str.find_last_not_of(WHITESPACES) + 1);
	return str;
}

// trim from beginning of string (left)
std::string &utils::ltrim(std::string &str) {
	str.erase(0, str.find_first_not_of(WHITESPACES));
	return str;
}

// trim from both ends of string (right then left)
std::string &utils::trim(std::string &str) {
	return ltrim(rtrim(str));
}
