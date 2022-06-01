//
// Created by Diana catherine Salamanca leguizamon on 5/19/22.
//

#include <string>
#include <vector>
#include <iostream> //delete
#include "utils.hpp"

void utils::removeWhiteSpaces(std::string str) {
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
//	std::cout << "res: " << str << std::endl; //delete, testing purposes
}

// https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
std::vector<std::string> utils::splitString(const std::string &str, char splitter) {
	std::vector<std::string> res;
	std::stringstream line(str);
	std::string segment;

	while (std::getline(line, segment, splitter)) {
		res.push_back(segment);
		std::cout << segment << std::endl; //delete, testing purposes
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


//check if this function is needed

void utils::trim(std::string &str, std::string chars) {
	std::size_t start = str.find_first_not_of(chars);
	if (start == std::string::npos) {
		start = 0;
	}
	std::size_t end = str.find_last_not_of(chars);
	if (end == std::string::npos) {
		str = ";";
		return;
	}
	std::size_t len = end - start + 1;
	str = str.substr(start, len);
}
