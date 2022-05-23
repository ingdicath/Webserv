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

/**
 * Put in another file option 1 for split
 */
std::vector<std::string> splitString2(std::string str, char splitter) {
	std::vector<std::string> res;
	std::string current;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == splitter) {
			if (!current.empty()) {
				res.push_back(current);
				current.clear();
			}
			continue;
		}
		current += str[i];
	}
	if (!current.empty())
		res.push_back(current);
	return res;
}

/**
 * Put in another file
 */

//void deleteLastChar(const std::string &line, char toSearch) {
//	size_t pos = line.find_last_of(toSearch);
//	if (pos != std::string::npos) {
//		line.erase(pos, pos + 1);
//	}
//}

// https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
/**
 * Put in another file option 2 for split
 */
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

//check

void utils::trim(std::string &str, std::string chars){
	std::size_t start = str.find_first_not_of(chars);
	if (start == std::string::npos){
		start = 0;
	}
	std::size_t end = str.find_last_not_of(chars);
	if (end == std::string::npos){
		str = ";";
		return;
	}
	std::size_t len = end - start + 1;
	str = str.substr(start, len);
}
